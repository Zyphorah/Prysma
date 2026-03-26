fn bool testNewDeleteFloatBasic()
{
   dec string[] feature = "1.testNewDeleteFloatBasic : ";
   call print(ref feature);
   dec ptr p = new float;
   aff p = 3.14;
   dec float value = unref p;
   delete p;
   if (value == 3.14) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatZero()
{
   dec string[] feature = "2.testNewDeleteFloatZero : ";
   call print(ref feature);
   dec ptr p = new float;
   aff p = 0.0;
   dec float value = unref p;
   delete p;
   if (value == 0.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatNegative()
{
   dec string[] feature = "3.testNewDeleteFloatNegative : ";
   call print(ref feature);
   dec ptr p = new float;
   dec float valueToAssign = 0.0 - 99.9;
   aff p = valueToAssign;
   dec float value = unref p;
   delete p;
   if (value == valueToAssign) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatSmallValue()
{
   dec string[] feature = "4.testNewDeleteFloatSmallValue : ";
   call print(ref feature);
   dec ptr p = new float;
   aff p = 0.001;
   dec float value = unref p;
   delete p;
   if (value == 0.001) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatReassignment()
{
   dec string[] feature = "5.testNewDeleteFloatReassignment : ";
   call print(ref feature);
   dec ptr p = new float;
   aff p = 1.1;
   aff p = 55.5;
   dec float value = unref p;
   delete p;
   if (value == 55.5) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteFloat()
{
   call print(call testNewDeleteFloatBasic()); call backSlashN();
   call print(call testNewDeleteFloatZero()); call backSlashN();
   call print(call testNewDeleteFloatNegative()); call backSlashN();
   call print(call testNewDeleteFloatSmallValue()); call backSlashN();
   call print(call testNewDeleteFloatReassignment()); call backSlashN();

   return 1;
}
