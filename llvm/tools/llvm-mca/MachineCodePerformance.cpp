//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// This file implements class MachineCodePerformance.
///
//===----------------------------------------------------------------------===//

#include "MachineCodePerformance.h"
#include "Views/InstructionView.h"

namespace llvm {
namespace mca {

MachineCodePerformance::MachineCodePerformance(Pipeline &Pipe) : P(Pipe) {}

void MachineCodePerformance::addSummaryView(std::unique_ptr<SummaryView> V) {
  P.addEventListener(V.get());
  summaryView = std::move(V);
}

void MachineCodePerformance::addResourcePressureView(
    std::unique_ptr<ResourcePressureView> V) {
  P.addEventListener(V.get());
  resourcePressureView = std::move(V);
}

void MachineCodePerformance::collectFileParameters() {
  DisplayValues DV = summaryView->getSummaryViewParameters();
  FP.Iterations = DV.Iterations;
  FP.Instructions = DV.TotalInstructions;
  FP.TotalCycles = DV.TotalCycles;
  FP.TotalUOps = DV.TotalUOps;
  FP.DispatchWidth = DV.DispatchWidth;
  FP.UOpsPerCycle = DV.UOpsPerCycle;
  FP.IPC = DV.IPC;
  FP.BlockRThroughput = DV.BlockRThroughput;
  resourcePressureView->getResourcePressurePerIter(FP.ResourcePressurePerIter);
}

MachineCodePerformance::FileParameters
MachineCodePerformance::getFileParameters() const {
  return FP;
}

void MachineCodePerformance::printColNamesOfResourcePressurePerIter(
    raw_ostream &OS, const MCSchedModel &SM) const {
  resourcePressureView->printColNamesOfResourcePressurePerIter(OS, SM);
}

void MachineCodePerformance::printResourcePressurePerIterWithoutColNames(
    raw_ostream &OS, const MCSchedModel &SM) const {
  resourcePressureView->printResourcePressurePerIterWithoutColNames(OS, SM);
}
} // namespace mca
} // namespace llvm
