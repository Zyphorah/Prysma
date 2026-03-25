
fn bool testNewDeleteBoolVrai()
{
   dec string[] functionnalite = "1.testNewDeleteBoolVrai : ";
   call print(ref functionnalite);
   dec ptr p = new bool;
   aff p = true;
   dec bool val = unref p;
   delete p;
   if (val == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteBoolFaux()
{
   dec string[] functionnalite = "2.testNewDeleteBoolFaux : ";
   call print(ref functionnalite);
   dec ptr p = new bool;
   aff p = false;
   dec bool val = unref p;
   delete p;
   if (val == false) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteBoolReassignment()
{
   dec string[] functionnalite = "3.testNewDeleteBoolReassignment : ";
   call print(ref functionnalite);
   dec ptr p = new bool;
   aff p = true;
   aff p = false;
   dec bool val = unref p;
   delete p;
   if (val == false) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteBoolReassignmentInverse()
{
   dec string[] functionnalite = "4.testNewDeleteBoolReassignmentInverse : ";
   call print(ref functionnalite);
   dec ptr p = new bool;
   aff p = false;
   aff p = true;
   dec bool val = unref p;
   delete p;
   if (val == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteBool()
{
   call print(call testNewDeleteBoolVrai()); call backSlashN();
   call print(call testNewDeleteBoolFaux()); call backSlashN();
   call print(call testNewDeleteBoolReassignment()); call backSlashN();
   call print(call testNewDeleteBoolReassignmentInverse()); call backSlashN();

   return 1;
}
