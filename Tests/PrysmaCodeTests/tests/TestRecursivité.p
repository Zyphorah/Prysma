scope {
   // Test 1 : Factorielle (5! = 120) - Récursion simple
   fn int fact(arg int n)
   {
      if (n <= 1) {
         return 1;
      }
      dec int m = n - 1;
      dec int r = call fact(m);
      return n * r;
   }

   fn bool testFact()
   {
      dec int fib = call fact(5);
      if (fib == 120) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   // Test 2 : fibonaci (fib(7) = 13) - Récursion multiple
   fn int fib(arg int n) {
      if (n <= 1) {
         return n;
      }
      dec int un = n - 1;
      dec int deux = n - 2;
      dec int fib_un = call fib(un);
      dec int fib_deux = call fib(deux);
      return fib_un + fib_deux;
   }


   fn bool testFib()
   {
      dec int fib = call fib(7);
      if (fib == 13) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn bool testRecursivite()
   {
      dec int fact = call fact(5);
      dec int fib = call fib(7);
      if ((fact == 120) && (fib == 13)) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn int main()
   {
      call printBool(call testFact());
      call printBool(call testFib());
      call printBool(call testRecursivite());

      return 1;
   }
}

