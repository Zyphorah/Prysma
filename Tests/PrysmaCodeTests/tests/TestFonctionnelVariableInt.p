scope {

   // Test 1 : Priorité standard (* gagne sur +)
   // Calcul : 10 + (2 * 5) = 20
   // Si la priorité est fausse (gauche vers droite) : (10 + 2) * 5 = 60
   fn bool testPrioriteSimple()
   {
      dec int a = 10;
      dec int b = 2;
      dec int c = 5;
      dec int d = a + b * c; 
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
   fn bool testPrioriteComplexe()
   {
      dec int a = 2;
      dec int b = 3;
      dec int c = 4;
      dec int d = 5;
      dec int resultat = a * b + c * d;


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
   fn bool testMiseAJourEtat()
   {
      dec int a = 1;
      dec int b = 5;
      
      aff a = 10; 
      
      dec int c = a + b;
      if (c == 15) {
         return true;
      } else {
         return false;
      }
      
      return false;
   }

   // Test 4 : Vérifier que les arguments peuvent passer dans une fonction 
   fn bool testPassArgFonction(arg int a, arg float b)
   {  
      if ((a == 10) && (b == 25.5)) {
         return true;
      } else {
         return false;
      }

      return false;
   }

   // Test 5 : Vérifier que les parenthèses sont respectées dans une expression complexe
   fn bool testProfondeurEquation()
   {
      dec float equation = ((1+2)*(3+4))/(5-6-7*(8-9));
      call printFloat(equation); // Affiche le résultat pour vérifier manuellement
      if (equation == 3.5) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn bool testAffectation()
   {
      dec int a = 5;
      aff a = 10; 
      if (a == 10) {
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
      dec int a = 10;
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
   fn bool testCopieVariable()
   {
      dec int a = 50;
      dec int b = a; // Doit copier 50 dans b
      
      aff a = 0; // On modifie a, b ne doit pas changer

      if (b == 50) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn bool testImbrication()
   {
      dec int a = 1;
      dec int b = 2;

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

   fn int main()
   {
      call printBool(call testPrioriteSimple());
      call printBool(call testPrioriteComplexe());
      call printBool(call testMiseAJourEtat());

      // false test : je dois corriger ça plus tard
      dec int a = 10; 
      dec float b = 25.5;
      call printBool(call testPassArgFonction(a,b)); // false
      
      // Appeler la méthode de nouveau pour voir si le contexte n'a pas changé
      dec int d = 10; 
      dec float e = 25.5;
      call printBool(call testPassArgFonction(d,e)); // false
      // le nombre est coupé, je ne vois pas la partie décimale donc false
      call printBool(call testProfondeurEquation()); // false 

      call printBool(call testAffectation());

      call printBool(call testAutoIncrement());
      call printBool(call testCopieVariable());

      call printBool(call testImbrication());
   
      // call testPassArgFonction(1,1); une erreur ici je dois corriger ça plus tard, il n'y a que les variables qui passe. 
      // chiffre négatif non pris en compte, je dois corriger ça aussi plus tard
      // Le lexer ne gère pas les nombres dans un nom de variable ou fonction
      // !false ne fonctionne pas, je dois corriger ça aussi plus tard 
      // Je ne peux pas utiliser les comparaison litéral directement en utilisant l'opérante logique true false je dois le stocker dans une variable avant 
      // Les fonctions ne peuvent pas être appelés dans une condition if bug lorsque je fait call fib(7) == 13; même dans une variable, un problème à investiguer. 
      
      return 1;
   }
}