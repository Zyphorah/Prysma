; ModuleID = 'BoucleWhile.c'
source_filename = "BoucleWhile.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define dso_local void @boucle() #0 {
entry:
; on initialise la variable i à 0, on alloue de la mémoire pour i et on stock la valeur 0 dans cette mémoire
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond

while.cond:                                    
  %0 = load i32, ptr %i, align 4 ; on charge la valeur de i dans le registre virtuel %0, qui est de type i32
  %cmp = icmp slt i32 %0, 10 ; on compare la valeur de i à 10, si i < 10 alors cmp = 1 sinon cmp = 0
  br i1 %cmp, label %while.body, label %while.end ; ici on utilise l'instruction br pour faire un branchement conditionnel
  ; si %cmp est égal à 1 (vrai) alors on saute vers le bloc while.body, sinon on saute vers le bloc while.end 

while.body:                                     
  %1 = load i32, ptr %i, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, ptr %i, align 4
  br label %while.cond, !llvm.loop !6

while.end:                                       
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

