// Test 1: Factorial (5! = 120) - Simple recursion
fn int32 factorial(arg int32 n)
{
   dec string[] feature = "1.factorial : ";
   call print(ref feature);
   if (n <= 1) {
      return 1;
   }
   dec int32 m = n - 1;
   dec int32 r = call factorial(m);
   return n * r;
}

fn bool testFactorial()
{
   dec string[] feature = "2.testFactorial : ";
   call print(ref feature);
   dec int32 result = call factorial(5);
   if (result == 120) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 2: Fibonacci (fib(7) = 13) - Multiple recursion
fn int32 fibonacci(arg int32 n) {
   dec string[] feature = "3.fibonacci : ";
   call print(ref feature);
   if (n <= 1) {
      return n;
   }
   dec int32 one = n - 1;
   dec int32 two = n - 2;
   dec int32 fib_one = call fibonacci(one);
   dec int32 fib_two = call fibonacci(two);
   return fib_one + fib_two;
}


fn bool testFibonacci()
{
   dec string[] feature = "4.testFibonacci : ";
   call print(ref feature);
   dec int32 result = call fibonacci(7);
   if (result == 13) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testSimpleRecursion()
{
   dec string[] feature = "5.testSimpleRecursion : ";
   call print(ref feature);
   dec int32 fact = call factorial(5);
   dec int32 fib = call fibonacci(7);
   if ((fact == 120) && (fib == 13)) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testRecursion()
{
   call print(call testFactorial()); call backSlashN();
   call print(call testFibonacci()); call backSlashN();
   call print(call testSimpleRecursion()); call backSlashN();

   return 1;
}
