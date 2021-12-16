//===----------------------- MachineCodePerformance.h --------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
///
/// MachineCodePerformance allows to collect machine code performance of an
/// input assembler file.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_MCA_MACHINECODEPERFORMANCE_H
#define LLVM_TOOLS_LLVM_MCA_MACHINECODEPERFORMANCE_H

#include "Views/ResourcePressureView.h"
#include "Views/SummaryView.h"
#include "llvm/MCA/Pipeline.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
namespace mca {

class MachineCodePerformance {
public:
  class FileParameters {
  public:
    SmallVector<std::string> ResourcePressurePerIter;
    unsigned Iterations;
    unsigned Instructions;
    unsigned TotalCycles;
    unsigned TotalUOps;
    unsigned DispatchWidth;
    double UOpsPerCycle;
    double IPC;
    double BlockRThroughput;
    FileParameters() : ResourcePressurePerIter() {}
  };

private:
  Pipeline &P;
  std::unique_ptr<SummaryView> summaryView;
  std::unique_ptr<ResourcePressureView> resourcePressureView;
  FileParameters FP;

public:
  MachineCodePerformance(Pipeline &Pipe);
  void addSummaryView(std::unique_ptr<SummaryView> V);
  void addResourcePressureView(std::unique_ptr<ResourcePressureView> V);
  void collectFileParameters();
  FileParameters getFileParameters() const;
  void printColNamesOfResourcePressurePerIter(raw_ostream &OS,
                                              const MCSchedModel &SM) const;
  void
  printResourcePressurePerIterWithoutColNames(raw_ostream &OS,
                                              const MCSchedModel &SM) const;
};
} // namespace mca
} // namespace llvm

#endif // LLVM_TOOLS_LLVM_MCA_MACHINECODEPERFORMANCE_H
