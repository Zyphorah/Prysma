fn bool testNewDeleteInt32Basic()
{
   dec string[] feature = "1.testNewDeleteInt32Basic : ";
   call print(ref feature);
   dec ptr p = new int32;
   aff p = 42;
   dec int32 value = unref p;
   delete p;
   if (value == 42) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt32Zero()
{
   dec string[] feature = "2.testNewDeleteInt32Zero : ";
   call print(ref feature);
   dec ptr p = new int32;
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

fn bool testNewDeleteInt32Negative()
{
   dec string[] feature = "3.testNewDeleteInt32Negative : ";
   call print(ref feature);
   dec ptr p = new int32;
   dec int32 valueToAssign = 0 - 100;
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

fn bool testNewDeleteInt32LargeValue()
{
   dec string[] feature = "4.testNewDeleteInt32LargeValue : ";
   call print(ref feature);
   dec ptr p = new int32;
   aff p = 2147483;
   dec int32 value = unref p;
   delete p;
   if (value == 2147483) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt32Reassignment()
{
   dec string[] feature = "5.testNewDeleteInt32Reassignment : ";
   call print(ref feature);
   dec ptr p = new int32;
   aff p = 10;
   aff p = 99;
   dec int32 value = unref p;
   delete p;
   if (value == 99) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteInt32()
{
   call print(call testNewDeleteInt32Basic()); call backSlashN();
   call print(call testNewDeleteInt32Zero()); call backSlashN();
   call print(call testNewDeleteInt32Negative()); call backSlashN();
   call print(call testNewDeleteInt32LargeValue()); call backSlashN();
   call print(call testNewDeleteInt32Reassignment()); call backSlashN();

   return 1;
}
