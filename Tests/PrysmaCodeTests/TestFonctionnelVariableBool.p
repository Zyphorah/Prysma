scope {

   fn bool testEtLogique()
   {
      dec string[] fonctionnalite = "1.testEtLogique : ";
      call printString(ref fonctionnalite);
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
      dec string[] fonctionnalite = "2.testOuLogique : ";
      call printString(ref fonctionnalite);
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
      dec string[] fonctionnalite = "3.testNonLogique : ";
      call printString(ref fonctionnalite);
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
      dec string[] fonctionnalite = "4.testNonLogiqueFaux : ";
      call printString(ref fonctionnalite);
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
      dec string[] fonctionnalite = "5.testOuLogiqueVariable : ";
      call printString(ref fonctionnalite);
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
      dec string[] fonctionnalite = "6.testEtLogiqueVariable : ";
      call printString(ref fonctionnalite);
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
      dec string[] fonctionnalite = "7.testNonEtLogiqueVariable : ";
      call printString(ref fonctionnalite);
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

   fn int32 main()
   {
      // Tester les bool 
      call printBool(call testEtLogique()); call backSlashN();
      call printBool(call testOuLogique()); call backSlashN();
      call printBool(call testNonLogique()); call backSlashN();
      call printBool(call testNonLogiqueFaux()); call backSlashN();
      call printBool(call testOuLogiqueVariable()); call backSlashN();
      call printBool(call testEtLogiqueVariable()); call backSlashN();
      call printBool(call testNonEtLogiqueVariable()); call backSlashN();

      return 1;
   }
}
