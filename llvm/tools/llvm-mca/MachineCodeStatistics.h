//===----------------------- Comparator.h --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// This file implements class MachineCodeStatistics.
///
/// MachineCodeStatistics allows to collect machine code statistics of an input
/// assembler file.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_MCA_MACHINECODESTATISTICS_H
#define LLVM_TOOLS_LLVM_MCA_MACHINECODESTATISTICS_H

#include "Views/ResourcePressureView.h"
#include "Views/SummaryView.h"
#include "llvm/MCA/Pipeline.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
namespace mca {

class MachineCodeStatistics {
public:
  class OneFileParameters {
  public:
    unsigned Iterations;
    unsigned Instructions;
    unsigned TotalCycles;
    unsigned TotalUOps;
    unsigned DispatchWidth;
    double UOpsPerCycle;
    double IPC;
    double BlockRThroughput;
    llvm::SmallVector<std::string> resoucePerIterations;

    OneFileParameters() : resoucePerIterations() {}
  };

private:
  Pipeline &P;
  std::unique_ptr<SummaryView> summaryView;
  std::unique_ptr<ResourcePressureView> resourcePressureView;
  OneFileParameters OFP;

public:
  MachineCodeStatistics(Pipeline &Pipe) : P(Pipe) {}

  void addSummaryView(std::unique_ptr<SummaryView> V) {
    P.addEventListener(V.get());
    summaryView = std::move(V);
  }

  void addResourcePressureView(std::unique_ptr<ResourcePressureView> V) {
    P.addEventListener(V.get());
    resourcePressureView = std::move(V);
  }

  void collectParametersToCompare();

  OneFileParameters getOneFileParameters() const { return OFP; }

  void printColNamesPerIter(llvm::raw_ostream &OS,
                            const MCSchedModel &SM) const {
    resourcePressureView->printColNamesPerIter(OS, SM);
  }
  void printValuesPerIter(llvm::raw_ostream &OS, const MCSchedModel &SM) const {
    resourcePressureView->printValuesPerIter(OS, SM);
  }
};

} // namespace mca
} // namespace llvm

#endif // LLVM_TOOLS_LLVM_MCA_MACHINECODESTATISTICS_H