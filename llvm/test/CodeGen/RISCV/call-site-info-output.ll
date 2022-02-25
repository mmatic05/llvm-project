;; Test riscv32:
; RUN: llc -mtriple=riscv32 -emit-call-site-info -stop-before=finalize-isel %s -o - | \
; RUN: llc -mtriple=riscv32 -emit-call-site-info -x='mir' -run-pass=finalize-isel -o -| FileCheck %s

;; Test riscv64:
; RUN: llc -mtriple=riscv64 -emit-call-site-info -stop-before=finalize-isel %s -o - | \
; RUN: llc -mtriple=riscv64 -emit-call-site-info -x='mir' -run-pass=finalize-isel -o -| FileCheck %s --check-prefix=CHECK64



;; Source:
;; extern void addi_instr (long int, long int, long int);
;; extern void add_instr (long int, long int, long int);
;; extern void andi_instr (long int, long int, long int);
;; extern void and_instr (long int, long int, long int);
;; extern void lui_instr (long int, long int, long int);
;; extern void or_instr (long int, long int, long int);
;; extern void ori_xori_instr (long int, long int, long int);
;; extern void slli_srli_srai_instr (long int, long int, long int);
;; extern void sll_srl_sra_instr (long int, long int, long int);
;; extern void sub_instr (long int, long int, long int);
;; extern void xor_instr (long int, long int, long int);
;; long int fn2 (long int a, long int b, long int c) 
;; {
;;  long int q = 2 * a;
;;  addi_instr (1,2,3); 
;;  add_instr (4,5,6); 
;;  andi_instr (7,8,9); 
;;  and_instr (10,11,12); 
;;  lui_instr (13,14,15); 
;;  or_instr (16,17,18); 
;;  ori_xori_instr (19,20,21); 
;;  slli_srli_srai_instr (22,23,24); 
;;  sll_srl_sra_instr (25,26,27); 
;;  sub_instr (28,29,30); 
;;  xor_instr (31,32,33); 
;;  return 0;
;; }
 
;; Test riscv32:
; CHECK: name: fn2
; CHECK: callSites:
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'
; CHECK-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK-NEXT:   arg: 0, reg: '$x10'
; CHECK-NEXT:   arg: 1, reg: '$x11'
; CHECK-NEXT:   arg: 2, reg: '$x12'

;; Test riscv64:
; CHECK64: name: fn2
; CHECK64: callSites:
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'
; CHECK64-NEXT: bb: {{.*}}, offset: {{.*}}, fwdArgRegs:
; CHECK64-NEXT:   arg: 0, reg: '$x10'
; CHECK64-NEXT:   arg: 1, reg: '$x11'
; CHECK64-NEXT:   arg: 2, reg: '$x12'


; Function Attrs: nounwind
define dso_local i32 @fn2(i32 %a, i32 %b, i32 %c) local_unnamed_addr #0 !dbg !8 {
entry:
  call void @llvm.dbg.value(metadata i32 %a, metadata !13, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 %b, metadata !14, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 %c, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata !DIArgList(i32 2, i32 %a), metadata !16, metadata !DIExpression(DW_OP_LLVM_arg, 0, DW_OP_LLVM_arg, 1, DW_OP_mul, DW_OP_stack_value)), !dbg !17
  tail call void @addi_instr(i32 1, i32 2, i32 3) #3, !dbg !17
  tail call void @add_instr(i32 4, i32 5, i32 6) #3, !dbg !17
  tail call void @andi_instr(i32 7, i32 8, i32 9) #3, !dbg !17
  tail call void @and_instr(i32 10, i32 11, i32 12) #3, !dbg !17
  tail call void @lui_instr(i32 13, i32 14, i32 15) #3, !dbg !17
  tail call void @or_instr(i32 16, i32 17, i32 18) #3, !dbg !17
  tail call void @ori_xori_instr(i32 19, i32 20, i32 21) #3, !dbg !17
  tail call void @slli_srli_srai_instr(i32 22, i32 23, i32 24) #3, !dbg !17
  tail call void @sll_srl_sra_instr(i32 25, i32 26, i32 27) #3, !dbg !17
  tail call void @sub_instr(i32 28, i32 29, i32 30) #3, !dbg !17
  tail call void @xor_instr(i32 31, i32 32, i32 33) #3, !dbg !17
  ret i32 0, !dbg !17
}

declare !dbg !30 dso_local void @addi_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !34 dso_local void @add_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !35 dso_local void @andi_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !36 dso_local void @and_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !37 dso_local void @lui_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !38 dso_local void @or_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !39 dso_local void @ori_xori_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !40 dso_local void @slli_srli_srai_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !41 dso_local void @sll_srl_sra_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !42 dso_local void @sub_instr(i32, i32, i32) local_unnamed_addr #1

declare !dbg !43 dso_local void @xor_instr(i32, i32, i32) local_unnamed_addr #1

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #2


!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 14.0.0", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/")
!2 = !{i32 7, !"Dwarf Version", i32 4}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 1, !"target-abi", !"ilp32d"}
!6 = !{i32 1, !"SmallDataLimit", i32 8}
!7 = !{!"clang version 14.0.0"}
!8 = distinct !DISubprogram(name: "fn2", scope: !1, file: !1, line: 14, type: !9, scopeLine: 15, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !12)
!9 = !DISubroutineType(types: !10)
!10 = !{!11, !11, !11, !11}
!11 = !DIBasicType(name: "long", size: 32, encoding: DW_ATE_signed)
!12 = !{!13, !14, !15, !16}
!13 = !DILocalVariable(name: "a", arg: 1, scope: !8, file: !1, line: 14, type: !11)
!14 = !DILocalVariable(name: "b", arg: 2, scope: !8, file: !1, line: 14, type: !11)
!15 = !DILocalVariable(name: "c", arg: 3, scope: !8, file: !1, line: 14, type: !11)
!16 = !DILocalVariable(name: "q", scope: !8, file: !1, line: 16, type: !11)
!17 = !DILocation(line: 0, scope: !8)
!30 = !DISubprogram(name: "addi_instr", scope: !1, file: !1, line: 1, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!31 = !DISubroutineType(types: !32)
!32 = !{null, !11, !11, !11}
!33 = !{}
!34 = !DISubprogram(name: "add_instr", scope: !1, file: !1, line: 2, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!35 = !DISubprogram(name: "andi_instr", scope: !1, file: !1, line: 3, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!36 = !DISubprogram(name: "and_instr", scope: !1, file: !1, line: 4, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!37 = !DISubprogram(name: "lui_instr", scope: !1, file: !1, line: 5, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!38 = !DISubprogram(name: "or_instr", scope: !1, file: !1, line: 6, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!39 = !DISubprogram(name: "ori_xori_instr", scope: !1, file: !1, line: 7, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!40 = !DISubprogram(name: "slli_srli_srai_instr", scope: !1, file: !1, line: 8, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!41 = !DISubprogram(name: "sll_srl_sra_instr", scope: !1, file: !1, line: 9, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!42 = !DISubprogram(name: "sub_instr", scope: !1, file: !1, line: 10, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
!43 = !DISubprogram(name: "xor_instr", scope: !1, file: !1, line: 11, type: !31, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !33)
