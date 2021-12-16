# The llvm-mca pretty printer 

The [__LLVM-MCA__](https://llvm.org/docs/CommandGuide/llvm-mca.html) is a performance analysis tool that uses information available in LLVM to statically measure the performance of machine code in a specific CPU.

__example.c :__


```
#include <stdio.h>

int sum(int a, int b) { return a + b; }

int main() {

  int e = 1;
  int k = 5;

  int s = sum(e, k);
  printf("sum(%d,%d)=%d \n", e, k, s);

  return 0;
}
```

```
$ clang example.c -o example.ll -S -emit-llvm 
$ llc example.ll -o example.s
$ llvm-mca example.s 
```
```
Iterations:        100
Instructions:      2800
Total Cycles:      2192
Total uOps:        4500

Dispatch Width:    6
uOps Per Cycle:    2.05
IPC:               1.28
Block RThroughput: 10.0


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 3      2     1.00           *            pushq	%rbp
 1      1     0.25                        movq	%rsp, %rbp
 1      1     1.00           *            movl	%edi, -8(%rbp)
 1      1     1.00           *            movl	%esi, -4(%rbp)
 1      5     0.50    *                   movl	-8(%rbp), %eax
 2      6     0.50    *                   addl	-4(%rbp), %eax
 2      6     0.50    *                   popq	%rbp
 3      7     1.00                  U     retq
 3      2     1.00           *            pushq	%rbp
 1      1     0.25                        movq	%rsp, %rbp
 1      1     0.25                        subq	$16, %rsp
 1      1     1.00           *            movl	$0, -16(%rbp)
 1      1     1.00           *            movl	$1, -8(%rbp)
 1      1     1.00           *            movl	$5, -4(%rbp)
 1      5     0.50    *                   movl	-8(%rbp), %edi
 1      5     0.50    *                   movl	-4(%rbp), %esi
 4      3     1.00                        callq	sum
 1      1     1.00           *            movl	%eax, -12(%rbp)
 1      5     0.50    *                   movl	-8(%rbp), %esi
 1      5     0.50    *                   movl	-4(%rbp), %edx
 1      5     0.50    *                   movl	-12(%rbp), %ecx
 1      1     0.25                        movabsq	$.L.str, %rdi
 1      1     0.25                        movb	$0, %al
 4      3     1.00                        callq	printf
 1      0     0.17                        xorl	%eax, %eax
 1      1     0.25                        addq	$16, %rsp
 2      6     0.50    *                   popq	%rbp
 3      7     1.00                  U     retq


Resources:
[0]   - SKLDivider
[1]   - SKLFPDivider
[2]   - SKLPort0
[3]   - SKLPort1
[4]   - SKLPort2
[5]   - SKLPort3
[6]   - SKLPort4
[7]   - SKLPort5
[8]   - SKLPort6
[9]   - SKLPort7


Resource pressure per iteration:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    
 -      -     4.71   4.72   7.00   7.00   10.00  4.73   4.84   7.00   

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    Instructions:
 -      -     0.27   0.44   0.60   0.01   1.00   0.27   0.02   0.39   pushq	%rbp
 -      -     0.44   0.27    -      -      -     0.29    -      -     movq	%rsp, %rbp
 -      -      -      -     0.02    -     1.00    -      -     0.98   movl	%edi, -8(%rbp)
 -      -      -      -      -      -     1.00    -      -     1.00   movl	%esi, -4(%rbp)
 -      -      -      -     0.96   0.04    -      -      -      -     movl	-8(%rbp), %eax
 -      -     0.28   0.27   0.04   0.96    -     0.18   0.27    -     addl	-4(%rbp), %eax
 -      -     0.27   0.29   0.01   0.99    -     0.28   0.16    -     popq	%rbp
 -      -     0.12   0.49   0.39   0.61    -     0.39   1.00    -     retq
 -      -     0.27   0.18   0.97   0.01   1.00   0.28   0.27   0.02   pushq	%rbp
 -      -     0.18   0.28    -      -      -     0.27   0.27    -     movq	%rsp, %rbp
 -      -     0.28   0.27    -      -      -     0.27   0.18    -     subq	$16, %rsp
 -      -      -      -     0.01   0.01   1.00    -      -     0.98   movl	$0, -16(%rbp)
 -      -      -      -     0.30    -     1.00    -      -     0.70   movl	$1, -8(%rbp)
 -      -      -      -     0.01    -     1.00    -      -     0.99   movl	$5, -4(%rbp)
 -      -      -      -     0.01   0.99    -      -      -      -     movl	-8(%rbp), %edi
 -      -      -      -     0.68   0.32    -      -      -      -     movl	-4(%rbp), %esi
 -      -     0.75   0.31   0.45    -     1.00   0.42   0.52   0.55   callq	sum
 -      -      -      -     0.30   0.25   1.00    -      -     0.45   movl	%eax, -12(%rbp)
 -      -      -      -     0.01   0.99    -      -      -      -     movl	-8(%rbp), %esi
 -      -      -      -     0.99   0.01    -      -      -      -     movl	-4(%rbp), %edx
 -      -      -      -     0.01   0.99    -      -      -      -     movl	-12(%rbp), %ecx
 -      -     0.17   0.29    -      -      -     0.26   0.28    -     movabsq	$.L.str, %rdi
 -      -     0.28   0.26    -      -      -     0.28   0.18    -     movb	$0, %al
 -      -     0.89   0.35   0.06    -     1.00   0.54   0.22   0.94   callq	printf
 -      -      -      -      -      -      -      -      -      -     xorl	%eax, %eax
 -      -     0.12   0.29    -      -      -     0.28   0.31    -     addq	$16, %rsp
 -      -     0.23   0.19   0.63   0.37    -     0.42   0.16    -     popq	%rbp
 -      -     0.16   0.54   0.55   0.45    -     0.30   1.00    -     retq
```


## The llvm-mca tool improvement  

The idea of improving the __llvm-mca tool__ is to introduce a __new option “-diff”__, that compares the statistics (results of the llvm-mca tool) to the attached assembler files. 
```
$ clang example.c -o example-O1.ll -S -emit-llvm -O1 
$ llc example-O1.ll -o example-O1.s -O1
$ clang example.c -o example-O2.ll -S -emit-llvm -O2 
$ llc example-O2.ll -o example-O2.s -O2
$ clang example.c -o example-O3.ll -S -emit-llvm -O3 
$ llc example-O3.ll -o example-O3.s -O3
$ llvm-mca example.s example-O1.s example-O2.s example-O3.s -diff
```
```
Input files:  
[f1]: example.s
[f2]: example-O1.s
[f3]: example-O2.s
[f4]: example-O3.s

Iterations:  100
                                   [f1]                          [f2]                          [f3]                          [f4]                          
Instructions                       2800                          1200                          1200                          1200                          
Total Cycles                       2192                          1096                          1096                          1096                          
Total uOps                         4500                          2200                          2200                          2200                          
Dispatch Width                     6                             6                             6                             6                             
uOps Per Cycle                     2.050000                      2.010000                      2.010000                      2.010000                      
IPC                                1.280000                      1.090000                      1.090000                      1.090000                      
Block RThroughp                    10.000000                     3.670000                      3.670000                      3.670000                      


Resource pressure per iteration:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    
[f1]: 
 -      -     4.71   4.72   7.00   7.00   10.00  4.73   4.84   7.00   
[f2]: 
 -      -     3.00   3.00   1.75   1.76   2.00   3.01   3.99   1.49   
[f3]: 
 -      -     3.00   3.00   1.75   1.76   2.00   3.01   3.99   1.49   
[f4]: 
 -      -     3.00   3.00   1.75   1.76   2.00   3.01   3.99   1.49   
 
```


Restrictions on the number of input files have also been introduced, without the option “-diff”,
mandatory and only one input file is allowed, while with the "-diff" option, more than one input 
assembler file is required.
```
$ llvm-mca example.s
$ llvm-mca example.s example-O1.s -diff
$ llvm-mca example.s example-O1.s example-O2.s
$ llvm-mca example.s example-O1.s example-O2.s example-O3.s -diff
 ```     
The reference to options “-o” and “-json” next to option “-diff” now refers to the output of the
comparison of input files, which is a consequence of option “-diff”.
```
$ llvm-mca example.s example-O1.s example-O2.s example-O3.s -diff -json
$ llvm-mca example.s example-O1.s example-O2.s example-O3.s -diff -o OutputFileName.txt
$ llvm-mca example.s example-O1.s example-O2.s example-O3.s -diff -json -o OutputFileName.json
```
```
{
  "File: 1": {
    "Block RThroughput": 10,
    "Dispatch Width": 6,
    "IPC": 1.2773722627737227,
    "Instructions": 2800,
    "Iterations": 100,
    "Name": "example.s",
    "Resource pressure per iteration": " - ,  - , 4.715000, 4.725000, 7.005000, 7.005000, 10.005000, 4.735000, 4.845000, 7.005000, ",
    "Total Cycles": 2192,
    "Total uOps": 4500,
    "uOps Per Cycle": 2.0529197080291972
  },
  "File: 2": {
    "Block RThroughput": 3.6666666666666665,
    "Dispatch Width": 6,
    "IPC": 1.0948905109489051,
    "Instructions": 1200,
    "Iterations": 100,
    "Name": "example-O1.s",
    "Resource pressure per iteration": " - ,  - , 3.005000, 3.005000, 1.755000, 1.765000, 2.005000, 3.015000, 3.995000, 1.495000, ",
    "Total Cycles": 1096,
    "Total uOps": 2200,
    "uOps Per Cycle": 2.0072992700729926
  },
  "File: 3": {
    "Block RThroughput": 3.6666666666666665,
    "Dispatch Width": 6,
    "IPC": 1.0948905109489051,
    "Instructions": 1200,
    "Iterations": 100,
    "Name": "example-O2.s",
    "Resource pressure per iteration": " - ,  - , 3.005000, 3.005000, 1.755000, 1.765000, 2.005000, 3.015000, 3.995000, 1.495000, ",
    "Total Cycles": 1096,
    "Total uOps": 2200,
    "uOps Per Cycle": 2.0072992700729926
  },
  "File: 4": {
    "Block RThroughput": 3.6666666666666665,
    "Dispatch Width": 6,
    "IPC": 1.0948905109489051,
    "Instructions": 1200,
    "Iterations": 100,
    "Name": "example-O3.s",
    "Resource pressure per iteration": " - ,  - , 3.005000, 3.005000, 1.755000, 1.765000, 2.005000, 3.015000, 3.995000, 1.495000, ",
    "Total Cycles": 1096,
    "Total uOps": 2200,
    "uOps Per Cycle": 2.0072992700729926
  }
}


```


## LLVM-MCA-PRETTY-PRINTER python script

New uitlity, __llvm-mca-pretty-printer (python script)__, depends on the llvm-mca tool. 
LLVM-MCA tool will be called within the llvm-mca-pretty-printer tool, which has a task to 
visualize the differences in the statistics of input assembler files (the “-diff” 
option has been added) or to display pictorial statistics of one input file (the option 
“-diff” is not given ). (Without the “-mtriple” and “-mcpu” options specified, the default 
tool-level options llvm-mca-pretty-printer are “-mcpu=skylake” and “-mtriple=x86_64-unknown-linux-gnu”. 
Also if we do not give the "-iterations" option, the default value for this option is 100. 
Python script is limited to working with 20 input files, due to the use of a color map for 
the purpose of drawing bars.):
```
$ llvm-mca-pretty-printer example.s 
```
![llvm-mca](https://user-images.githubusercontent.com/84574066/143688094-faadfbab-d9e1-4455-94f5-0e4e87c6a0e7.png)


```
$ llvm-mca-pretty-printer example.s example-O1.s example-O2.s example-O3.s -diff
```
![llvm-mca-diff](https://user-images.githubusercontent.com/84574066/143687788-b1a5d756-5da9-4ad3-bd05-d2c87e200dc5.png)



### Applying the script to assembly code obtained from [test_case](https://bugs.llvm.org/attachment.cgi?id=25056)
```   
$ llc store.ll -o store.s
$ llc -O3 -aarch64-enable-gep-opt=true store.ll -o store.s 
$ llvm-mca-pretty-printer -mtriple=aarch64 -mcpu=cyclone store.s storeEnableGrepOpt.s -diff
```  
![llvm-mca-diff](https://user-images.githubusercontent.com/84574066/143687767-92b04a98-c53f-4366-9093-ba8cd3cf73e9.png)



### Applying the script to assembly code obtained from [llvm/tools/llc/llc.cpp](https://github.com/llvm/llvm-project/blob/main/llvm/tools/llc/llc.cpp) 

Display of tool operation if any of the optimizations -O0, -O1, -O2 or -O3 was used during the compilation, ie display of the difference in statistics.

![llvm-mca-diff](https://user-images.githubusercontent.com/84574066/143687739-39b4dead-db3f-4b46-8996-68b35c0d8f99.png)



