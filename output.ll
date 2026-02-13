; ModuleID = 'output'
source_filename = "output"
target triple = "x86_64-pc-linux-gnu"

declare i32 @printInt(i32)

declare float @printFloat(float)

declare i1 @printBool(i1)

define i32 @fib(i32 %n) {
entry:
  %0 = alloca i32, align 4
  store i32 %n, ptr %0, align 4
  %n1 = load i32, ptr %0, align 4
  %autocast = sitofp i32 %n1 to float
  %cmptmp = fcmp ule float %autocast, 1.000000e+00
  br i1 %cmptmp, label %if, label %else

if:                                               ; preds = %entry
  %n2 = load i32, ptr %0, align 4
  ret i32 %n2
  br label %endif

else:                                             ; preds = %entry
  %un = alloca i32, align 4
  %deux = alloca i32, align 4
  %fib_un = alloca i32, align 4
  %fib_deux = alloca i32, align 4
  %res = alloca i32, align 4
  %n3 = load i32, ptr %0, align 4
  %autocast4 = sitofp i32 %n3 to float
  %subtmp = fsub float %autocast4, 1.000000e+00
  %autocast5 = fptosi float %subtmp to i32
  store i32 %autocast5, ptr %un, align 4
  %n6 = load i32, ptr %0, align 4
  %autocast7 = sitofp i32 %n6 to float
  %subtmp8 = fsub float %autocast7, 2.000000e+00
  %autocast9 = fptosi float %subtmp8 to i32
  store i32 %autocast9, ptr %deux, align 4
  %un10 = load i32, ptr %un, align 4
  %autocast11 = sitofp i32 %un10 to float
  %resultat_appel = call i32 @fib(float %autocast11)
  store i32 %resultat_appel, ptr %fib_un, align 4
  %deux12 = load i32, ptr %deux, align 4
  %autocast13 = sitofp i32 %deux12 to float
  %resultat_appel14 = call i32 @fib(float %autocast13)
  store i32 %resultat_appel14, ptr %fib_deux, align 4
  %fib_un15 = load i32, ptr %fib_un, align 4
  %fib_deux16 = load i32, ptr %fib_deux, align 4
  %autocast17 = sitofp i32 %fib_un15 to float
  %autocast18 = sitofp i32 %fib_deux16 to float
  %addtmp = fadd float %autocast17, %autocast18
  %autocast19 = fptosi float %addtmp to i32
  store i32 %autocast19, ptr %res, align 4
  br label %endif

endif:                                            ; preds = %else, %if
  %res20 = load i32, ptr %res, align 4
  ret i32 %res20
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %res = alloca i32, align 4
  %autocast = fptosi float 7.000000e+00 to i32
  store i32 %autocast, ptr %n, align 4
  %n1 = load i32, ptr %n, align 4
  %autocast2 = sitofp i32 %n1 to float
  %resultat_appel = call i32 @fib(float %autocast2)
  store i32 %resultat_appel, ptr %res, align 4
  %res3 = load i32, ptr %res, align 4
  %autocast4 = sitofp i32 %res3 to float
  %resultat_appel5 = call i32 @printInt(float %autocast4)
  %0 = fptosi float 0.000000e+00 to i32
  ret i32 %0
}
