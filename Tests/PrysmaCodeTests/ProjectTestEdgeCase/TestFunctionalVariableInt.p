
// Test 1 : Priorité standard (* gagne sur +)
// Calcul : 10 + (2 * 5) = 20
// Si la priorité est fausse (gauche vers droite) : (10 + 2) * 5 = 60
fn bool testPrioriteSimple_int()
{
   dec string[] functionnalite = "1.testPrioriteSimple : ";
   call print(ref functionnalite);
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

// Test 2 : Priorité complexe avec 4 variables
// Calcul : (2 * 3) + (4 * 5) = 6 + 20 = 26
// Si faux : 2 * 3 + 4 = 10, puis 10 * 5 = 50
fn bool testPrioriteComplex_int()
{
   dec string[] functionnalite = "2.testPrioriteComplex : ";
   call print(ref functionnalite);
   dec int32 a = 2;
   dec int32 b = 3;
   dec int32 c = 4;
   dec int32 d = 5;
   dec int32 resultat = a * b + c * d;


   if (resultat == 26) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test 3 : Vérification de la mise à jour d'état (aff) avant calcul
// On initialise a à 1, puis on le change à 10 avant le calcul
// Calcul : 10 + 5 = 15
fn bool testMiseAJourEtat_int()
{
   dec string[] functionnalite = "3.testMiseAJourEtat : ";
   call print(ref functionnalite);
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

// Test 4 : Vérifier que les arguments peuvent passer dans une function 
fn bool testPassArgFunction_int(arg int32 a, arg float b)
{  
   dec string[] functionnalite = "4.testPassArgFunction : ";
   call print(ref functionnalite);
   if ((a == 10) && (b == 25.5)) {
      return true;
   } else {
      return false;
   }

   return false;
}

// Test 5 : Vérifier que les parenthèses sont respectées dans une expression complexe
fn bool testDepthEquation_int()
{
   dec string[] functionnalite = "5.testDepthEquation : ";
   call print(ref functionnalite);
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
   dec string[] functionnalite = "6.testAssignment : ";
   call print(ref functionnalite);
   dec int32 a = 5;
   aff a = 10; 
   if (a == 10) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test critique : Auto-référence (a = a + 1)
// Vérifie que le compiler load 'a' avant d'écraser 'a'
fn bool testAutoIncrement_int()
{
   dec string[] functionnalite = "7.testAutoIncrement : ";
   call print(ref functionnalite);
   dec int32 a = 10;
   aff a = a + 1; // Si le load/store est mal ordonné, ça plante ou donne 1

   if (a == 11) {
      return true;
   } else {
      return false;
   }
   return false;
}

// Test critique : Variable vers Variable
// Vérifie que la valeur est bien copiée, pas juste liée
fn bool testCopieVariable_int()
{
      dec string[] functionnalite = "8.testCopieVariable : ";
      call print(ref functionnalite);
   dec int32 a = 50;
   dec int32 b = a; // Doit copier 50 dans b
   
   aff a = 0; // On modifie a, b ne doit pas changer

   if (b == 50) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testImbrication_int()
{
      dec string[] functionnalite = "9.testImbrication : ";
      call print(ref functionnalite);
   dec int32 a = 1;
   dec int32 b = 2;

   if (a == 1) {
      if (b == 2) {
            return true; // Doit arriver ici
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
   call print(call testPrioriteSimple_int()); call backSlashN();
   call print(call testPrioriteComplex_int()); call backSlashN();
   call print(call testMiseAJourEtat_int()); call backSlashN();

   call print(call testPassArgFunction_int(10,25.5)); call backSlashN();
   
   // Caller la méthode de nouveau pour voir si le contexte n'a pas changé
   call print(call testPassArgFunction_int(10,25.5)); call backSlashN();
   call print(call testDepthEquation_int()); call backSlashN();

   call print(call testAssignment_int()); call backSlashN();

   call print(call testAutoIncrement_int()); call backSlashN();
   call print(call testCopieVariable_int()); call backSlashN();

   call print(call testImbrication_int()); call backSlashN();

   return 1;
}

