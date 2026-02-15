scope {

   // Test 1 : Priorité standard (* gagne sur +)
   // Calcul : 10.0 + (2.0 * 5.0) = 20.0
   // Si la priorité est fausse (gauche vers droite) : (10.0 + 2.0) * 5.0 = 60.0
   fn bool testPrioriteSimple()
   {
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

   // Test 2 : Priorité complexe avec 4 variables
   // Calcul : (2.0 * 3.0) + (4.0 * 5.0) = 6.0 + 20.0 = 26.0
   // Si faux : 2.0 * 3.0 + 4.0 = 10.0, puis 10.0 * 5.0 = 50.0
   fn bool testPrioriteComplexe()
   {
      dec float a = 2.0;
      dec float b = 3.0;
      dec float c = 4.0;
      dec float d = 5.0;
      dec float resultat = a * b + c * d;


      if (resultat == 26.0) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   // Test 3 : Vérification de la mise à jour d'état (aff) avant calcul
   // On initialise a à 1.0, puis on le change à 10.0 avant le calcul
   // Calcul : 10.0 + 5.0 = 15.0
   fn bool testMiseAJourEtat()
   {
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

   // Test 4 : Vérifier que les arguments peuvent passer dans une fonction 
   fn bool testPassArgFonction(arg float a, arg float b)
   {  
      if ((a == 10.0) && (b == 25.5)) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   // Test 5 : Vérifier que les parenthèses sont respectées dans une expression complexe
   fn bool testProfondeurEquation()
   {
      dec float equation = ((1.0+2.0)*(3.0+4.0))/(5.0-6.0-7.0*(8.0-9.0));
      if (equation == 3.5) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn bool testAffectation()
   {
      dec float a = 5.0;
      aff a = 10.0; 
      if (a == 10.0) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   // Test critique : Auto-référence (a = a + 1)
   // Vérifie que le compilateur charge 'a' avant d'écraser 'a'
   fn bool testAutoIncrement()
   {
      dec float a = 10.0;
      aff a = a + 1.0; // Si le load/store est mal ordonné, ça plante ou donne 1

      if (a == 11.0) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   // Test critique : Variable vers Variable
   // Vérifie que la valeur est bien copiée, pas juste liée
   fn bool testCopieVariable()
   {
      dec float a = 50.0;
      dec float b = a; // Doit copier 50.0 dans b
      
      aff a = 0.0; // On modifie a, b ne doit pas changer

      if (b == 50.0) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn bool testImbrication()
   {
      dec float a = 1.0;
      dec float b = 2.0;

      if (a == 1.0) {
         if (b == 2.0) {
             return true; // Doit arriver ici
         } else {
             return false;
         }
      } else {
         return false;
      }
      return false;
   }

   fn int main()
   {
      call printBool(call testPrioriteSimple());
      call printBool(call testPrioriteComplexe());
      call printBool(call testMiseAJourEtat());

      // false test : je dois corriger ça plus tard
      dec float a = 10.0; 
      dec float b = 25.5;
      call printBool(call testPassArgFonction(a,b)); 
      
      // Appeler la méthode de nouveau pour voir si le contexte n'a pas changé
      dec float d = 10.0; 
      dec float e = 25.5;
      call printBool(call testPassArgFonction(d,e)); 
      call printBool(call testProfondeurEquation()); 

      call printBool(call testAffectation());

      call printBool(call testAutoIncrement());
      call printBool(call testCopieVariable());

      call printBool(call testImbrication());
    
      return 1;
   }
}
