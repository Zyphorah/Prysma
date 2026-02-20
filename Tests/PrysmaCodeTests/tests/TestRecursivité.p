scope {
   // Test 1 : Factorielle (5! = 120) - Récursion simple
   fn int32 fact(arg int32 n)
   {
      if (n <= 1) {
         return 1;
      }
      dec int32 m = n - 1;
      dec int32 r = call fact(m);
      return n * r;
   }

   fn bool testFact()
   {
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
      dec int32 fib = call fib(7);
      if (fib == 13) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn bool testRecursivite()
   {
      dec int32 fact = call fact(5);
      dec int32 fib = call fib(7);
      if ((fact == 120) && (fib == 13)) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn int32 main()
   {
      call printBool(call testFact());
      call printBool(call testFib());
      call printBool(call testRecursivite());

      return 1;
   }
}

