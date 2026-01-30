; ModuleID = 'PrysmaModule'
source_filename = "PrysmaModule"
target triple = "x86_64-pc-linux-gnu"

@.str = private constant [14 x i8] c"Resultat: %f\0A\00"

define i32 @main() {
entry:
  %teste = alloca float, align 4
  store double 1.300000e+01, ptr %teste, align 8
  %teste_value = load float, ptr %teste, align 4
  %formatPtr = getelementptr [14 x i8], ptr @.str, i64 0, i64 0
  %0 = call i32 (ptr, ...) @printf(ptr %formatPtr, float %teste_value)
  %resultInt = fptosi float %teste_value to i32
  ret i32 %resultInt
}

declare i32 @printf(ptr, ...)
