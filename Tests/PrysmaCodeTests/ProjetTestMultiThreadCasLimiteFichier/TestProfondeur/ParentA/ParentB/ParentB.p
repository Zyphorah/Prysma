include "EnfantA.p";
include "EnfantB.p";
include "EnfantC.p";
include "EnfantD.p";
include "EnfantE.p";
include "ParentC/ParentC.p";

fn void printParentB()
{
    call printEnfantA_B(); call backSlashN();
    call printEnfantB_B(); call backSlashN();
    call printEnfantC_B(); call backSlashN();
    call printEnfantD_B(); call backSlashN();
    call printEnfantE_B(); call backSlashN();
    call printParentC(); call backSlashN();
}
