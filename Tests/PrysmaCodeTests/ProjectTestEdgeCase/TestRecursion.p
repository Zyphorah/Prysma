
// Test 1 : Factorielle (5! = 120) - Récursion simple
fn int32 fact(arg int32 n)
{
   dec string[] functionnalite = "1.fact : ";
   call print(ref functionnalite);
   if (n <= 1) {
      return 1;
   }
   dec int32 m = n - 1;
   dec int32 r = call fact(m);
   return n * r;
}

fn bool testFact()
{
   dec string[] functionnalite = "2.testFact : ";
   call print(ref functionnalite);
   dec int32 fib = call fact(5);
   if (fib == 120) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 2 : fibonaci (fib(7) = 13) - Récursion multiple
fn int32 fib(arg int32 n) {
   dec string[] functionnalite = "3.fib : ";
   call print(ref functionnalite);
   if (n <= 1) {
      return n;
   }
   dec int32 un = n - 1;
   dec int32 deux = n - 2;
   dec int32 fib_un = call fib(un);
   dec int32 fib_deux = call fib(deux);
   return fib_un + fib_deux;
}


fn bool testFib()
{
   dec string[] functionnalite = "4.testFib : ";
   call print(ref functionnalite);
   dec int32 fib = call fib(7);
   if (fib == 13) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testRecursiviteSimple()
{
   dec string[] functionnalite = "5.testRecursiviteSimple : ";
   call print(ref functionnalite);
   dec int32 fact = call fact(5);
   dec int32 fib = call fib(7);
   if ((fact == 120) && (fib == 13)) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testRecursivite()
{
   call print(call testFact()); call backSlashN();
   call print(call testFib()); call backSlashN();
   call print(call testRecursiviteSimple()); call backSlashN();

   return 1;
}
