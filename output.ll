; ModuleID = 'output'
source_filename = "output"
target triple = "x86_64-pc-linux-gnu"

declare i32 @printInt(i32)

declare float @printFloat(float)

declare i1 @printBool(i1)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %tableau = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  store i32 0, ptr %a, align 4
  %ptr_case = getelementptr [3 x i32], ptr %tableau, i32 0, i32 0
  %a1 = load i32, ptr %a, align 4
  store i32 %a1, ptr %ptr_case, align 4
  %ptr_case2 = getelementptr [3 x i32], ptr %tableau, i32 0, i32 1
  store i32 2, ptr %ptr_case2, align 4
  %ptr_case3 = getelementptr [3 x i32], ptr %tableau, i32 0, i32 2
  store i32 3, ptr %ptr_case3, align 4
  %0 = getelementptr [3 x i32], ptr %tableau, i32 0, i32 10
  store i32 1, ptr %0, align 4
  %1 = getelementptr [3 x i32], ptr %tableau, i32 0, i32 0
  store i32 1, ptr %1, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i4 = load i32, ptr %i, align 4
  %icmp_lt = icmp slt i32 %i4, 4
  br i1 %icmp_lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %i5 = load i32, ptr %i, align 4
  %tableau6 = getelementptr [3 x i32], ptr %tableau, i32 0, i32 %i5
  %tableau7 = load i32, ptr %tableau6, align 4
  %resultat_appel = call i32 @printInt(i32 %tableau7)
  %i8 = load i32, ptr %i, align 4
  %iadd = add i32 %i8, 1
  store i32 %iadd, ptr %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret i32 0
}
