include "EnfantA.p";
include "EnfantB.p";
include "EnfantC.p";
include "EnfantD.p";
include "EnfantE.p";
include "ParentB/ParentB.p";

fn void printParentA()
{
    call printEnfantA(); call backSlashN();
    call printEnfantB(); call backSlashN();
    call printEnfantC(); call backSlashN();
    call printEnfantD(); call backSlashN();
    call printEnfantE(); call backSlashN();
    call printParentB(); call backSlashN();
}