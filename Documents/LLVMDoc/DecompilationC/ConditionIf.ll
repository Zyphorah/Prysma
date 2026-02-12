; ModuleID = 'ConditionIf.c'
source_filename = "ConditionIf.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"if\00", align 1 
@.str.1 = private unnamed_addr constant [5 x i8] c"else\00", align 1

; Code source décompilé à partir du fichier ConditionIf.c
; Analyse détaillé de la condition if-else pour comprendre le fonctionnement en profondeur dans l'objectif de l'implémenter dans mon compilateur
define dso_local void @condition() #0 {
entry:
 ; Le bloc entry est le point d'entrée c'est là qu'on va faire les allocations de variables et les initialisations
  %x = alloca i32, align 4 ; on alloue une variable x de type i32 (entier de 32 bits)
  store i32 10, ptr %x, align 4 ; on stock la valeur 10 dans la variable x
  %0 = load i32, ptr %x, align 4 ; on charge la valeur de x dans la variable %0, qui est de type i32
  ; trois instruction importante alloca, store et load sont utilisée pour gérer les variables en mémoire
  
  %cmp = icmp slt i32 %0, 20 ; on compare la valeur de x à 20, si x < 20 alors cmp = 1 sinon cmp = 0
  ; on utilise icmp pour faire la comparaison entre les deux valeurs 
  ; on stock le résultat de la comparaison dans la variable %cmp, qui est de type i1 (boolean) 1 ou 0 

  br i1 %cmp, label %if.then, label %if.else ; ici on utilise l'instruction br pour faire un branchement conditionnel
  ; en fait c'est comme l'instruction jmp en assembleur, ça permet de faire un saut ver une étiquette (label) en fonction de la valeur de %cmp
  ; si %cmp est égal à 1 (vrai) alors on saute vers le bloc if.then, sinon on saute vers le bloc if.else
if.then:                                          ; Bloc if sert de flag jump si la condition est vérifiée
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %if.end

if.else:                                          ; Bloc else sert de flag jump si la condition n'est pas vérifiée
  %call1 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  br label %if.end

if.end:                                           ; Bloc de fin de la condition if-else, sert de flag jump pour les deux blocs précédents
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
