// Test 1: Standard priority (* wins over +)
// Calculation: 10 + (2 * 5) = 20
// If priority is wrong (left to right): (10 + 2) * 5 = 60
fn bool testSimplePriority_int()
{
   dec string[] feature = "1.testSimplePriority_int : ";
   call print(ref feature);
   dec int32 a = 10;
   dec int32 b = 2;
   dec int32 c = 5;
   dec int32 d = a + b * c; 
   if (d == 20) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 2: Complex priority with 4 variables
// Calculation: (2 * 3) + (4 * 5) = 6 + 20 = 26
// If false: 2 * 3 + 4 = 10, then 10 * 5 = 50
fn bool testComplexPriority_int()
{
   dec string[] feature = "2.testComplexPriority_int : ";
   call print(ref feature);
   dec int32 a = 2;
   dec int32 b = 3;
   dec int32 c = 4;
   dec int32 d = 5;
   dec int32 result = a * b + c * d;


   if (result == 26) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 3: Verification of state update (aff) before calculation
// We initialize a to 1, then change it to 10 before calculation
// Calculation: 10 + 5 = 15
fn bool testStateUpdate_int()
{
   dec string[] feature = "3.testStateUpdate_int : ";
   call print(ref feature);
   dec int32 a = 1;
   dec int32 b = 5;
   
   aff a = 10; 
   
   dec int32 c = a + b;
   if (c == 15) {
      return true;
   } else {
      return false;
   }
   
   return false;
}

// Test 4: Check that arguments can be passed to a function 
fn bool testPassArgFunction_int(arg int32 a, arg float b)
{  
   dec string[] feature = "4.testPassArgFunction : ";
   call print(ref feature);
   if ((a == 10) && (b == 25.5)) {
      return true;
   } else {
      return false;
   }

   return false;
}

// Test 5: Check that parentheses are respected in a complex expression
fn bool testDepthEquation_int()
{
   dec string[] feature = "5.testDepthEquation : ";
   call print(ref feature);
   dec float equation = ((1.0+2.0)*(3.0+4.0))/(5.0-6.0-7.0*(8.0-9.0));
   if (equation == 3.5) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testAssignment_int()
{
   dec string[] feature = "6.testAssignment : ";
   call print(ref feature);
   dec int32 a = 5;
   aff a = 10; 
   if (a == 10) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Critical test: Self-reference (a = a + 1)
// Checks that the compiler loads 'a' before overwriting 'a'
fn bool testAutoIncrement_int()
{
   dec string[] feature = "7.testAutoIncrement : ";
   call print(ref feature);
   dec int32 a = 10;
   aff a = a + 1; // If load/store is badly ordered, it crashes or gives 1

   if (a == 11) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Critical test: variable to variable
// Checks that the value is correctly copied, not just linked
fn bool testVariableCopy_int()
{
      dec string[] feature = "8.testVariableCopy_int : ";
      call print(ref feature);
   dec int32 a = 50;
   dec int32 b = a; // Must copy 50 into b
   
   aff a = 0; // We modify a, b must not change

   if (b == 50) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNesting_int()
{
      dec string[] feature = "9.testNesting_int : ";
      call print(ref feature);
   dec int32 a = 1;
   dec int32 b = 2;

   if (a == 1) {
      if (b == 2) {
            return true; // Must arrive here
      } else {
            return false;
      }
   } else {
      return false;
   }
   return false;
}

fn int32 testFunctionalVariableInt()
{
   call print(call testSimplePriority_int()); call backSlashN();
   call print(call testComplexPriority_int()); call backSlashN();
   call print(call testStateUpdate_int()); call backSlashN();

   call print(call testPassArgFunction_int(10,25.5)); call backSlashN();
   
   // Call the method again to see if the context hasn't changed
   call print(call testPassArgFunction_int(10,25.5)); call backSlashN();
   call print(call testDepthEquation_int()); call backSlashN();

   call print(call testAssignment_int()); call backSlashN();

   call print(call testAutoIncrement_int()); call backSlashN();
   call print(call testVariableCopy_int()); call backSlashN();

   call print(call testNesting_int()); call backSlashN();

   return 1;
}

