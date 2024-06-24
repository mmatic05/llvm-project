//===- MachineCheckDebugify.cpp - Check debug info ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file This checks debug info after mir-debugify (+ pass-to-test). Currently
/// it simply checks the integrity of line info in DILocation and
/// DILocalVariable which mir-debugifiy generated before.
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Module.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"

#define DEBUG_TYPE "mir-check-debugify"

using namespace llvm;

namespace {

static bool checkMIRDebugifyMetadata(MachineModuleInfo &MMI, Module &M) {
    NamedMDNode *NMD = M.getNamedMetadata("llvm.mir.debugify");
    if (!NMD) {
      errs() << "WARNING: Please run mir-debugify to generate "
                "llvm.mir.debugify metadata first.\n";
      return false;
    }


    auto getDebugifyOperand = [&](unsigned Idx) -> unsigned {
      return mdconst::extract<ConstantInt>(NMD->getOperand(Idx)->getOperand(0))
          ->getZExtValue();
    };
    assert(NMD->getNumOperands() == 2 &&
           "llvm.mir.debugify should have exactly 2 operands!");
    unsigned NumLines = getDebugifyOperand(0);
    unsigned NumVars = getDebugifyOperand(1);
    BitVector MissingLines{NumLines, true};
    BitVector MissingVars{NumVars, true};

    for (Function &F : M.functions()) {
      MachineFunction *MF = MMI.getMachineFunction(F);
      if (!MF)
        continue;
      for (MachineBasicBlock &MBB : *MF) {
        // Find missing lines.
        // TODO: Avoid meta instructions other than dbg_val.
        for (MachineInstr &MI : MBB) {
          if (MI.isDebugValue())
            continue;
          const DebugLoc DL = MI.getDebugLoc();
          if (DL && DL.getLine() != 0) {
            MissingLines.reset(DL.getLine() - 1);
            continue;
          }

          if (!DL) {
            errs() << "WARNING: Instruction with empty DebugLoc in function ";
            errs() << F.getName() << " --";
            MI.print(errs());
          }
        }

        // Find missing variables.
        // TODO: Handle DBG_INSTR_REF which is under an experimental option now.
        for (MachineInstr &MI : MBB) {
          if (!MI.isDebugValue())
            continue;
          const DILocalVariable *LocalVar = MI.getDebugVariable();
          unsigned Var = ~0U;

          (void)to_integer(LocalVar->getName(), Var, 10);
          assert(Var <= NumVars && "Unexpected name for DILocalVariable");
          MissingVars.reset(Var - 1);
        }
      }
    }

    bool Fail = false;
    for (unsigned Idx : MissingLines.set_bits()) {
      errs() << "WARNING: Missing line " << Idx + 1 << "\n";
      Fail = true;
    }

    for (unsigned Idx : MissingVars.set_bits()) {
      errs() << "WARNING: Missing variable " << Idx + 1 << "\n";
      Fail = true;
    }
    errs() << "Machine IR debug info check: ";
    errs() << (Fail ? "FAIL" : "PASS") << "\n";

    return false; 
  }

static bool checkMIRInstructions(const DebugMIRInstMap &DILocsBefore,
                              const DebugMIRInstMap &DILocsAfter,
                              const WeakMIRInstValueMap &InstToDelete,
                              StringRef NameOfWrappedPass) {
  bool Preserved = true;
  for (auto &L : DILocsAfter) {
    // Instr have dbgLoc after pass
    if (L.second != 0)
      continue;

    auto MIRInstrAfter = L.first;

    // In order to avoid pointer reuse/recycling, skip the values that might
    // have been deleted during a pass.
    auto WeakInstrPtr = InstToDelete.find(MIRInstrAfter);
    unsigned debugInstrNum = MIRInstrAfter->getDebugInstrNum();
    if (WeakInstrPtr != InstToDelete.end() && WeakInstrPtr->second != debugInstrNum)
      continue;

    auto InstrIt = DILocsBefore.find(MIRInstrAfter);

    // Wrapped pass added new instr without dbgLoc
    if (InstrIt == DILocsBefore.end())
      continue; 

    // Instr didn't have dbgLoc before pass
    if (InstrIt->second == 0)
      continue;

    Preserved = false;
    errs() << "WARNING: Missing line " << InstrIt->second << "\n";  
  }

  return Preserved;
}


static bool checkMIRDebugInfoMetadata(MachineModuleInfo &MMI, Module &M, StringRef NameOfWrappedPass, DebugInfoPerMIRPass * DbgInfoBeforeMIRPass) {
    DebugMIRInstMap DbgInfoAfterMIRPass;
    
    for (Function &F : M.functions()) {
      MachineFunction *MF = MMI.getMachineFunction(F);
      if (!MF)
        continue;
      for (MachineBasicBlock &MBB : *MF) {

        // Cllect dbgLoc.
        for (MachineInstr &MI : MBB) {
          if (MI.isDebugValue())
            continue;
          
          const DebugLoc DL = MI.getDebugLoc();
          bool HasLoc = DL && DL.getLine() != 0;
          if (HasLoc) {
            DbgInfoAfterMIRPass.insert({&MI,DL.getLine()});         
          } else {
            DbgInfoAfterMIRPass.insert({&MI,0});         
          }
        }
      }
    }

  bool ResultForInsts = checkMIRInstructions(DbgInfoBeforeMIRPass->DILocations, DbgInfoAfterMIRPass, DbgInfoBeforeMIRPass->InstToDelete, NameOfWrappedPass);

  if (ResultForInsts)
    errs() << "Machine IR debug info check: PASS\n";
  else
    errs() << "Machine IR debug info check: FAIL\n";

  return false;
  }

struct CheckDebugMachineModule : public ModulePass {
  bool runOnModule(Module &M) override {

  MachineModuleInfo &MMI = getAnalysis<MachineModuleInfoWrapperPass>().getMMI();

  if (Mode == MIRDebugifyMode::SyntheticDebugInfo) {
     return checkMIRDebugifyMetadata(MMI, M);
    } else if (Mode == MIRDebugifyMode::OriginalDebugInfo) {
     return checkMIRDebugInfoMetadata(MMI, M, NameOfWrappedPass, DbgInfoBeforeMIRPass);
    }

  return false;
  }

  CheckDebugMachineModule() : ModulePass(ID) {
    Mode = MIRDebugifyMode::SyntheticDebugInfo;
  }

  CheckDebugMachineModule(DebugInfoPerMIRPass *DbgInfoPerMIRPass, MIRDebugifyMode DebugifyMode, const std::string &Banner) : ModulePass(ID) {
    DbgInfoBeforeMIRPass = DbgInfoPerMIRPass;
    Mode = DebugifyMode;
    NameOfWrappedPass = Banner;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<MachineModuleInfoWrapperPass>();
    AU.setPreservesAll();
  }

  static char ID; // Pass identification.
  MIRDebugifyMode Mode;

  //valid only for origin debugify mode
  DebugInfoPerMIRPass *DbgInfoBeforeMIRPass;
  std::string NameOfWrappedPass;
};
char CheckDebugMachineModule::ID = 0;

} // end anonymous namespace

INITIALIZE_PASS_BEGIN(CheckDebugMachineModule, DEBUG_TYPE,
                      "Machine Check Debug Module", false, false)
INITIALIZE_PASS_END(CheckDebugMachineModule, DEBUG_TYPE,
                    "Machine Check Debug Module", false, false)

ModulePass *llvm::createCheckDebugMachineModulePass(DebugInfoPerMIRPass *DbgInfoPerMIRPass, MIRDebugifyMode Mode, const std::string &Banner) {
  return new CheckDebugMachineModule(DbgInfoPerMIRPass, Mode, Banner);
}
