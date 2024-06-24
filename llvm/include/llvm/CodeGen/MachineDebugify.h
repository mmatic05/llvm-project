//===- MachineDebugify.h - Check debug info preservation in optimizations on MIR level --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file Interface to the `mir debugify` synthetic/original debug info testing
/// utility.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_MIR_DEBUGIFY_H
#define LLVM_TRANSFORMS_UTILS_MIR_DEBUGIFY_H

#include "llvm/ADT/MapVector.h"
#include <string>

using DebugMIRInstMap = llvm::MapVector<llvm::MachineInstr *, unsigned /*debugLoc line or 0*/>;
using WeakMIRInstValueMap =
    llvm::MapVector<llvm::MachineInstr *, unsigned /*debugInstrNum*/>;

struct DebugInfoPerMIRPass {
  DebugMIRInstMap DILocations;
  WeakMIRInstValueMap InstToDelete;
};

enum class MIRDebugifyMode { NoDebugify, SyntheticDebugInfo, OriginalDebugInfo };

namespace llvm {

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_MIR_DEBUGIFY_H
