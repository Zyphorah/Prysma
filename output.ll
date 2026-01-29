; ModuleID = 'PrysmaModule'
source_filename = "PrysmaModule"
target triple = "x86_64-pc-linux-gnu"

@.str = private constant [14 x i8] c"Resultat: %f\0A\00"

define i32 @main() !dbg !2 {
entry:
  %teste = alloca float, align 4, !debug_line !3
  store double 1.300000e+01, ptr %teste, align 8, !debug_line !3
  %teste_value = load float, ptr %teste, align 4, !debug_line !3
  %formatPtr = getelementptr [14 x i8], ptr @.str, i64 0, i64 0, !debug_line !3
  %0 = call i32 (ptr, ...) @printf(ptr %formatPtr, float %teste_value), !debug_line !3
  %resultInt = fptosi float %teste_value to i32, !debug_line !3
  ret i32 %resultInt, !debug_line !3
}

declare !dbg !4 i32 @printf(ptr, ...)

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "Prysma Compiler", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!1 = !DIFile(filename: "output.ll", directory: ".")
!2 = distinct !DISubprogram(name: "main", scope: null, file: !1, line: 1, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0)
!3 = !{i32 1}
!4 = distinct !DISubprogram(name: "printf", scope: null, file: !1, line: 1, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0)
