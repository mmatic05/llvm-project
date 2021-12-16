# RUN: llvm-mca -diff %p/Inputs/diff-option-file-1.s %p/Inputs/diff-option-file-2.s 2>&1 | FileCheck %s

# CHECK:      Input files:
# CHECK-NEXT: [f1]: diff-option-file-1.s
# CHECK-NEXT: [f2]: diff-option-file-2.s

# CHECK:      Iterations:  100

# CHECK:                                              [f1]:                         [f2]:                         
# CHECK-NEXT: Instructions:                           1100                          600
# CHECK-NEXT: Total Cycles:                           1097                          897
# CHECK-NEXT: Total uOps:                             1900                          1400
# CHECK-NEXT: Dispatch Width:                         6                             6
# CHECK-NEXT: uOps Per Cycle:                         1.73                          1.56
# CHECK-NEXT: IPC:                                    1.00                          0.67
# CHECK-NEXT: Block RThroughput:                      3.17                          2.33 

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]
# CHECK-NEXT: [f1]:
# CHECK-NEXT:  -      -     2.72   2.76   1.66   1.68   3.00   2.76   2.76   1.66
# CHECK-NEXT: [f2]:
# CHECK-NEXT:  -      -     1.11   1.95   1.33   1.34   2.00   1.95   1.99   1.33

