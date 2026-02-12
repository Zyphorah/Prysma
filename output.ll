; ModuleID = 'output'
source_filename = "output"
target triple = "x86_64-pc-linux-gnu"

declare i32 @printInt(i32)

declare i32 @printFloat(i32)

define i32 @test() {
entry:
  %0 = fptosi float 1.000000e+01 to i32
  ret i32 %0
}

define i32 @main() {
entry:
  %resultat = alloca i32, align 4
  %autocast = fptosi float 4.500000e+01 to i32
  store i32 %autocast, ptr %resultat, align 4
  %resultat_appel = call i32 @test()
  %resultat1 = load i32, ptr %resultat, align 4
  %resultat_appel2 = call i32 @printInt(i32 %resultat1)
  %resultat3 = load i32, ptr %resultat, align 4
  ret i32 %resultat3
}
