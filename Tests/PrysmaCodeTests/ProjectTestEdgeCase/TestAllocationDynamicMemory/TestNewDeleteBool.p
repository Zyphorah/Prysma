fn bool testNewDeleteBoolTrue()
{
   dec string[] feature = "1.testNewDeleteBoolTrue : ";
   call print(ref feature);
   dec ptr p = new bool;
   aff p = true;
   dec bool value = unref p;
   delete p;
   if (value == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteBoolFalse()
{
   dec string[] feature = "2.testNewDeleteBoolFa/lse : ";
   call print(ref feature);
   dec ptr p = new bool;
   aff p = false;
   dec bool value = unref p;
   delete p;
   if (value == false) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteBoolReassignment()
{
   dec string[] feature = "3.testNewDeleteBoolReassignment : ";
   call print(ref feature);
   dec ptr p = new bool;
   aff p = true;
   aff p = false;
   dec bool value = unref p;
   delete p;
   if (value == false) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteBoolReassignmentInverse()
{
   dec string[] feature = "4.testNewDeleteBoolReassignmentInverse : ";
   call print(ref feature);
   dec ptr p = new bool;
   aff p = false;
   aff p = true;
   dec bool value = unref p;
   delete p;
   if (value == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteBool()
{
   call print(call testNewDeleteBoolTrue()); call backSlashN();
   call print(call testNewDeleteBoolFalse()); call backSlashN();
   call print(call testNewDeleteBoolReassignment()); call backSlashN();
   call print(call testNewDeleteBoolReassignmentInverse()); call backSlashN();

   return 1;
}
