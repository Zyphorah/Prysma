; ModuleID = 'output'
source_filename = "output"
target triple = "x86_64-pc-linux-gnu"

define i32 @teste() {
entry:
  %a = alloca float, align 4
  %multmp = fmul float 1.000000e+01, 4.200000e+01
  store float %multmp, ptr %a, align 4
  ret float 1.000000e+00
}

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %addtmp = fadd float 1.000000e+01, 2.340000e+02
  store float %addtmp, ptr %a, align 4
  %multmp = fmul float 2.340000e+02, 4.320000e+02
  %subtmp = fsub float 2.340000e+02, %multmp
  %subtmp1 = fsub float 3.420000e+02, 4.200000e+01
  %subtmp2 = fsub float %subtmp, %subtmp1
  %addtmp3 = fadd float 1.000000e+01, %subtmp2
  store float %addtmp3, ptr %b, align 4
  store float 1.000000e+01, ptr %a, align 4
  %a4 = load float, ptr %a, align 4
  %addtmp5 = fadd float 1.300000e+01, %a4
  store float %addtmp5, ptr %b, align 4
  ret float 1.000000e+01
}
