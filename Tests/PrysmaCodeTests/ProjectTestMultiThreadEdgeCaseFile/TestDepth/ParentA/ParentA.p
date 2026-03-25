include "ChildA.p";
include "ChildB.p";
include "ChildC.p";
include "ChildD.p";
include "ChildE.p";
include "ParentB/ParentB.p";

fn void printParentA()
{
    call printChildA(); call backSlashN();
    call printChildB(); call backSlashN();
    call printChildC(); call backSlashN();
    call printChildD(); call backSlashN();
    call printChildE(); call backSlashN();
    call printParentB(); call backSlashN();
}