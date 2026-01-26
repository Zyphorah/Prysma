; ModuleID = 'PrysmaModule'
source_filename = "PrysmaModule"

@.str = private constant [14 x i8] c"Resultat: %f\0A\00"

define i32 @main() {
entry:
  %formatPtr = getelementptr [14 x i8], ptr @.str, i64 0, i64 0
  %0 = call i32 (ptr, ...) @printf(ptr %formatPtr, double -1.610000e+01)
  %resultInt = fptosi double -1.610000e+01 to i32
  ret i32 %resultInt
}

declare i32 @printf(ptr, ...)
