// Dans le futur les tests pourront avoir une condition if, pour le moment je suis obligé de passer manuellement chaque test
// afin de déterminer si le résultat est bon 

scope {

   // Test 1 : Priorité standard (* gagne sur +)
   // Calcul : 10 + (2 * 5) = 20
   // Si la priorité est fausse (gauche vers droite) : (10 + 2) * 5 = 60
   fn int testPrioriteSimple()
   {
      dec int a = 10;
      dec int b = 2;
      dec int c = 5;
      dec int d = a + b * c; 
      
      call printInt(d); // Résultat attendu : 20
      return d;
   }

   // Test 2 : Priorité complexe avec 4 variables
   // Calcul : (2 * 3) + (4 * 5) = 6 + 20 = 26
   // Si faux : 2 * 3 + 4 = 10, puis 10 * 5 = 50
   fn int testPrioriteComplexe()
   {
      dec int a = 2;
      dec int b = 3;
      dec int c = 4;
      dec int d = 5;
      dec int resultat = a * b + c * d;

      call printInt(resultat); // Résultat attendu : 26
      return resultat;
   }

   // Test 3 : Vérification de la mise à jour d'état (aff) avant calcul
   // On initialise a à 1, puis on le change à 10 avant le calcul
   // Calcul : 10 + 5 = 15
   fn int testMiseAJourEtat()
   {
      dec int a = 1;
      dec int b = 5;
      
      aff a = 10; 
      
      dec int c = a + b;
      call printInt(c); // Résultat attendu : 15
      return c;
   }

   // Test 4 : Vérifier que les arguments peuvent passer dans une fonction 
   fn int testPassArgFonction(arg int a, arg float b)
   {  
      call printInt(a); // devrait afficher 10
      call printFloat(b); // devrait afficher 25.5

      return a;
   }

   fn int main()
   {
      call testPrioriteSimple();
      call testPrioriteComplexe();
      call testMiseAJourEtat();
      dec int a = 10; 
      dec int b = 25.5;
      call testPassArgFonction(a,b); 
      
      // Appeler la méthode de nouveau pour voir si le contexte n'a pas changé
      dec int d = 10; 
      dec int e = 25.5;
      call testPassArgFonction(d,e);

      // call testPassArgFonction(1,1); une erreur ici je dois corriger ça plus tard, il n'y a que les variables qui passe. 
      // Je dois aussi accepter le retour d'une fonction dans une variable, ce n'est pas programmé encore. dec int a = call retourFonction();

      return 1;
   }
}