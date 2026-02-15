; ModuleID = 'Tableau.c'
source_filename = "Tableau.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@__const.tableau.tableau = private unnamed_addr constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], align 16

; Function Attrs: noinline nounwind optnone uwtable
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

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

