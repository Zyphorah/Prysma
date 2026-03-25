; ModuleID = 'Tableau.c'
source_filename = "Tableau.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Tableau de 5 entiers initialisé avec les valeurs 1, 2, 3, 4, 5
; Les tableau en llvm sont représenté sous forme de type [N*T] où N est le nombre d'éléments et T est le type des éléments
; On utilise 2 index pour accéder à un élément d'un tableau, le premier est l'index pour la position d'octet 00 00 00 00 / 00 00 00 00 / 00 00 00 00/ 
; on laisse le premier index à 0 mais ça peux être utile pour des cas particulier 
; Le deuxième c'est la navigation d'un bloc complet en mémoire index i pour accéder à l'élément i du tableau 
; on utilise une instruction getelementptr pour calculer l'adresse de l'élément i du tableau, en utilisant les deux index pour naviguer dans la mémoire
; Le fonctionnement derière l'accès à la mémoire du tableau est fait avec une formule mathématique pour calculer l'adresse de l'élément i du tableau, 
; la formule est la suivante : adresse de l'élément i = adresse de base du tableau + (i * taille de l'élément)
; on peux naviguer jump vers les bonnes adresse mémoire en utilisant cette formule mathématique. Mais c'est l'outils gep qui s'occupe de faire le calcule pour nous. 

@__const.tableau.tableau = private unnamed_addr constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], align 16

define dso_local void @tableau() #0 {
entry:
  %tableau = alloca [5 x i32], align 16
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %tableau, ptr align 16 @__const.tableau.tableau, i64 20, i1 false)
  %arrayidx = getelementptr inbounds [5 x i32], ptr %tableau, i64 0, i64 0
  store i32 10, ptr %arrayidx, align 16
  %arrayidx1 = getelementptr inbounds [5 x i32], ptr %tableau, i64 0, i64 1
  store i32 20, ptr %arrayidx1, align 4
  ret void
}

declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

