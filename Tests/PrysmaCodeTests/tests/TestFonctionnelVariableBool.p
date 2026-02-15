scope {

   fn bool testEtLogique()
   {
      dec bool a = true && true; 

      if (a == true) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn bool testOuLogique()
   {
      dec bool a = true || false; 

      if (a == true) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn bool testNonLogique()
   {
      dec bool a = true; 
      dec bool b = !a; 

      if (b == false) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn bool testNonLogiqueFaux()
   {
      dec bool a = !false; 

      if (a == true) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn bool testOuLogiqueVariable()
   {
      dec bool a = true; 
      dec bool b = false;

      dec bool d = a || b;

      if (d == true) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn bool testEtLogiqueVariable()
   {
      dec bool a = true; 
      dec bool b = false;

      dec bool d = a && b;

      if (d == false) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn bool testNonEtLogiqueVariable()
   {
      dec bool a = true; 
      dec bool b = false;

      dec bool d = !a && b;

      if (d == false) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   fn int main()
   {
      // Tester les bool 
      call printBool(call testEtLogique()); 
      call printBool(call testOuLogique());
      call printBool(call testNonLogique()); // false
      call printBool(call testNonLogiqueFaux()); // false
      call printBool(call testOuLogiqueVariable());
      call printBool(call testEtLogiqueVariable());
      call printBool(call testNonEtLogiqueVariable());

      return 1;
   }
}
