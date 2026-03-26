fn bool testNewDeleteInt64Basic()
{
   dec string[] feature = "1.testNewDeleteInt64Basic : ";
   call print(ref feature);
   dec ptr p = new int64;
   aff p = 100;
   dec int32 value = unref p;
   delete p;
   if (value == 100) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64Zero()
{
   dec string[] feature = "2.testNewDeleteInt64Zero : ";
   call print(ref feature);
   dec ptr p = new int64;
   aff p = 0;
   dec int32 value = unref p;
   delete p;
   if (value == 0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64Negative()
{
   dec string[] feature = "3.testNewDeleteInt64Negative : ";
   call print(ref feature);
   dec ptr p = new int64;
   dec int32 valueToAssign = 0 - 500;
   aff p = valueToAssign;
   dec int32 value = unref p;
   delete p;
   if (value == valueToAssign) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64LargeValue()
{
   dec string[] feature = "4.testNewDeleteInt64LargeValue : ";
   call print(ref feature);
   dec ptr p = new int64;
   aff p = 9999999;
   dec int32 value = unref p;
   delete p;
   if (value == 9999999) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64Reassignment()
{
   dec string[] feature = "5.testNewDeleteInt64Reassignment : ";
   call print(ref feature);
   dec ptr p = new int64;
   aff p = 1;
   aff p = 777;
   dec int32 value = unref p;
   delete p;
   if (value == 777) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteInt64()
{
   call print(call testNewDeleteInt64Basic()); call backSlashN();
   call print(call testNewDeleteInt64Zero()); call backSlashN();
   call print(call testNewDeleteInt64Negative()); call backSlashN();
   call print(call testNewDeleteInt64LargeValue()); call backSlashN();
   call print(call testNewDeleteInt64Reassignment()); call backSlashN();

   return 1;
}
