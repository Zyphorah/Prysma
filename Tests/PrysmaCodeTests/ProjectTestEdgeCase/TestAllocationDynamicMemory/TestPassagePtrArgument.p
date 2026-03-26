fn bool verifyInt32(arg ptr pointer)
{
   dec int32 value = unref pointer;
   if (value == 50) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPassPtrInt32ByArgument()
{
   dec string[] feature = "1.testPassPtrInt32ByArgument : ";
   call print(ref feature);
   dec ptr p = new int32;
   aff p = 50;
   dec bool result = call verifyInt32(p);
   delete p;
   if (result == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool verifyValueZero(arg ptr pointer)
{
   dec int32 value = unref pointer;
   if (value == 0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPassPtrValueZeroByArgument()
{
   dec string[] feature = "2.testPassPtrValueZeroByArgument : ";
   call print(ref feature);
   dec ptr p = new int32;
   aff p = 0;
   dec bool result = call verifyValueZero(p);
   delete p;
   if (result == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool verifyBool(arg ptr pointer)
{
   dec bool value = unref pointer;
   if (value == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPassPtrBoolByArgument()
{
   dec string[] feature = "3.testPassPtrBoolByArgument : ";
   call print(ref feature);
   dec ptr p = new bool;
   aff p = true;
   dec bool result = call verifyBool(p);
   delete p;
   if (result == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 readAndAdd(arg ptr ptrA, arg ptr ptrB)
{
   dec int32 a = unref ptrA;
   dec int32 b = unref ptrB;
   dec int32 sum = a + b;
   return sum;
}

fn bool testPassTwoPtrByArgument()
{
   dec string[] feature = "4.testPassTwoPtrByArgument : ";
   call print(ref feature);
   dec ptr p1 = new int32;
   dec ptr p2 = new int32;
   aff p1 = 30;
   aff p2 = 70;
   dec int32 result = call readAndAdd(p1, p2);
   delete p1;
   delete p2;
   if (result == 100) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 readAndReturn(arg ptr pointer)
{
   dec int32 value = unref pointer;
   return value;
}

fn bool verifyReturn(arg ptr pointer)
{
   dec int32 value = call readAndReturn(pointer);
   if (value == 999) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPassPtrToTwoFunctions()
{
   dec string[] feature = "5.testPassPtrToTwoFunctions : ";
   call print(ref feature);
   dec ptr p = new int32;
   aff p = 999;
   dec bool result = call verifyReturn(p);
   delete p;
   if (result == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testPassPtrArgument()
{
   call print(call testPassPtrInt32ByArgument()); call backSlashN();
   call print(call testPassPtrValueZeroByArgument()); call backSlashN();
   call print(call testPassPtrBoolByArgument()); call backSlashN();
   call print(call testPassTwoPtrByArgument()); call backSlashN();
   call print(call testPassPtrToTwoFunctions()); call backSlashN();

   return 1;
}
