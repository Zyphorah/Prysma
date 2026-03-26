// Test 1: Standard precedence (* wins over +)
// Calculation: 10.0 + (2.0 * 5.0) = 20.0
// If precedence is wrong (left to right): (10.0 + 2.0) * 5.0 = 60.0
fn bool testSimplePrecedence_float()
{
   dec string[] feature = "1.testSimplePrecedence : ";
   call print(ref feature);
   dec float a = 10.0;
   dec float b = 2.0;
   dec float c = 5.0;
   dec float d = a + b * c; 
   if (d == 20.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 2: Complex precedence with 4 variables
// Calculation: (2.0 * 3.0) + (4.0 * 5.0) = 6.0 + 20.0 = 26.0
// If false: 2.0 * 3.0 + 4.0 = 10.0, then 10.0 * 5.0 = 50.0
fn bool testComplexPrecedence_float()
{
   dec string[] feature = "2.testComplexPrecedence : ";
   call print(ref feature);
   dec float a = 2.0;
   dec float b = 3.0;
   dec float c = 4.0;
   dec float d = 5.0;
   dec float result = a * b + c * d;

   if (result == 26.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 3: check state update (aff) before calculation
// Initialize a to 1.0, then change it to 10.0 before calculation
// Calculation: 10.0 + 5.0 = 15.0
fn bool testStateUpdate_float()
{
   dec string[] feature = "3.testStateUpdate : ";
   call print(ref feature);
   dec float a = 1.0;
   dec float b = 5.0;
   
   aff a = 10.0; 
   
   dec float c = a + b;
   if (c == 15.0) {
      return true;
   } else {
      return false;
   }
   
   return false;
}

// Test 4: Check that arguments can be passed to a function
fn bool testPassArgFunction_float(arg float a, arg float b)
{  
   dec string[] feature = "4.testPassArgFunction : ";
   call print(ref feature);
   if ((a == 10.0) && (b == 25.5)) {
      return true;
   } else {
      return false;
   }

   return false;
}

// Test 5: Check that parentheses are respected in a complex expression
fn bool testDepthEquation_float()
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

fn bool testAssignment_float()
{
   dec string[] feature = "6.testAssignment : ";
   call print(ref feature);
   dec float a = 5.0;
   aff a = 10.0; 
   if (a == 10.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Critical test: Self-reference (a = a + 1)
// Checks that the compiler loads 'a' before overwriting 'a'
fn bool testAutoIncrement_float()
{
   dec string[] feature = "7.testAutoIncrement : ";
   call print(ref feature);
   dec float a = 10.0;
   aff a = a + 1.0; // If load/store is misordered, it fails or gives 1

   if (a == 11.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Critical test: variable to variable
// Checks that the value is copied, not just linked
fn bool testCopyVariable_float()
{
   dec string[] feature = "8.testCopyVariable : ";
   call print(ref feature);
   dec float a = 50.0;
   dec float b = a; // Must copy 50.0 into b
   
   aff a = 0.0; // Modify a, b must not change

   if (b == 50.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNesting_float()
{
   dec string[] feature = "9.testNesting : ";
   call print(ref feature);
   dec float a = 1.0;
   dec float b = 2.0;

   if (a == 1.0) {
      if (b == 2.0) {
            return true; // Must reach here
      } else {
            return false;
      }
   } else {
      return false;
   }
   return false;
}

fn int32 testFunctionalVariableFloat()
{
   call print(call testSimplePrecedence_float()); call backSlashN();
   call print(call testComplexPrecedence_float()); call backSlashN();
   call print(call testStateUpdate_float()); call backSlashN();

   // false test: I must fix this later
   dec float a = 10.0; 
   dec float b = 25.5;
   call print(call testPassArgFunction_float(a,b)); call backSlashN();
   
   // Call the method again to see if the context has not changed
   dec float d = 10.0; 
   dec float e = 25.5;
   call print(call testPassArgFunction_float(d,e)); call backSlashN();
   call print(call testDepthEquation_float()); call backSlashN();

   call print(call testAssignment_float()); call backSlashN();

   call print(call testAutoIncrement_float()); call backSlashN();
   call print(call testCopyVariable_float()); call backSlashN();

   call print(call testNesting_float()); call backSlashN();
   
   return 1;
}

