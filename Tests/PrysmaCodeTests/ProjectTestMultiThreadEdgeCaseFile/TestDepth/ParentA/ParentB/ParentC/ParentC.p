include "ChildA.p";
include "ChildB.p";
include "ChildC.p";
include "ChildD.p";
include "ChildE.p";

fn void printParentC()
{
    call printChildA_C(); call backSlashN();
    call printChildB_C(); call backSlashN();
    call printChildC_C(); call backSlashN();
    call printChildD_C(); call backSlashN();
    call printChildE_C(); call backSlashN();
}
