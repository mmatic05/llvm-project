//===----------------------- Comparator.cpp --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// This file implements the Comparator interface.
///
//===----------------------------------------------------------------------===//

#include "MachineCodeStatistics.h"
#include "Views/InstructionView.h"

namespace llvm {
namespace mca {

void MachineCodeStatistics::collectParametersToCompare() {

  summaryView->getDisplayValues(
      OFP.Instructions, OFP.Iterations, OFP.TotalCycles, OFP.DispatchWidth,
      OFP.TotalUOps, OFP.IPC, OFP.UOpsPerCycle, OFP.BlockRThroughput);

  resourcePressureView->getResourcePressurePerIter(OFP.resoucePerIterations);
}

} // namespace mca
} // namespace llvm