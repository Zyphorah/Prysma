fn bool testEtLogique()
{
   dec string[] functionnalite = "1.testEtLogique : ";
   call print(ref functionnalite);
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
   dec string[] functionnalite = "2.testOuLogique : ";
   call print(ref functionnalite);
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
   dec string[] functionnalite = "3.testNonLogique : ";
   call print(ref functionnalite);
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
   dec string[] functionnalite = "4.testNonLogiqueFaux : ";
   call print(ref functionnalite);
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
   dec string[] functionnalite = "5.testOuLogiqueVariable : ";
   call print(ref functionnalite);
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
   dec string[] functionnalite = "6.testEtLogiqueVariable : ";
   call print(ref functionnalite);
   dec bool a = true; 
   dec bool b = false;

   dec bool d = a && b;
  // temporaire test
   if (d == false) {
      return true;
   } else {
      return false;
   }

   return false;
}

fn bool testNonEtLogiqueVariable()
{
   dec string[] functionnalite = "7.testNonEtLogiqueVariable : ";
   call print(ref functionnalite);
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

fn int32 testFunctionalVariableBool()
{
   // Tester les bool 
   call print(call testEtLogique()); call backSlashN();
   call print(call testOuLogique()); call backSlashN();
   call print(call testNonLogique()); call backSlashN();
   call print(call testNonLogiqueFaux()); call backSlashN();
   call print(call testOuLogiqueVariable()); call backSlashN();
   call print(call testEtLogiqueVariable()); call backSlashN();
   call print(call testNonEtLogiqueVariable()); call backSlashN();

   return 1;
}
