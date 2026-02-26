include "EnfantA.p";
include "EnfantB.p";
include "EnfantC.p";
include "EnfantD.p";
include "EnfantE.p";

fn void printParentC()
{
    call printEnfantA_C(); call backSlashN();
    call printEnfantB_C(); call backSlashN();
    call printEnfantC_C(); call backSlashN();
    call printEnfantD_C(); call backSlashN();
    call printEnfantE_C(); call backSlashN();
}
