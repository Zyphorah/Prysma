include "ChildA.p";
include "ChildB.p";
include "ChildC.p";
include "ChildD.p";
include "ChildE.p";
include "ParentC/ParentC.p";

fn void printParentB()
{
    call printChildA_B(); call backSlashN();
    call printChildB_B(); call backSlashN();
    call printChildC_B(); call backSlashN();
    call printChildD_B(); call backSlashN();
    call printChildE_B(); call backSlashN();
    call printParentC(); call backSlashN();
}
