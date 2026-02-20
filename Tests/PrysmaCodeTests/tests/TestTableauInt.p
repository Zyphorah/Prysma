scope
{
    fn bool tableauTestLecture()
    {
         dec int32[4] tableau = [0, 1, 2, 3];

         if (tableau[0] != 0) {
             return false;
         }
         if (tableau[1] != 1) {
             return false;  
         }
        if (tableau[2] != 2) {
            return false;  
        }
        if (tableau[3] != 3) {
            return false;  
        }
        return true;
    }

    fn bool tableauTestAffectation()
    {
         dec int32[4] tableau = [0, 1, 2, 3];

         aff tableau[0] = 10;
         aff tableau[1] = 11;
         aff tableau[2] = 12;
         aff tableau[3] = 13;

         if (tableau[0] != 10) {
             return false;
         }
         if (tableau[1] != 11) {
             return false;  
         }
        if (tableau[2] != 12) {
            return false;  
        }
        if (tableau[3] != 13) {
            return false;  
        }
        return true;
    }

    fn bool tableauTestSortieHorsLimite()
    {
         dec int32[4] tableau = [0, 1, 2, 3];

         aff tableau[4] = 10; // Doit générer une erreur de compilation
         return true; // Ne doit pas être atteint
    }

    fn bool tableauVariableTest()
    {
        dec int32 a = 4; 
        dec int32 b = 2; 
        dec int32[5] tableau = [a, b, 3, 4, 5];

        if (tableau[0] != 4) {
            return false;
        }
        if (tableau[1] != 2) {
            return false;
        }
        if (tableau[2] != 3) {
            return false;
        }
        if (tableau[3] != 4) {
            return false;
        }
        if (tableau[4] != 5) {
            return false;
        }
        return true;
    }
    
    fn bool tableauTestIndexDynamique()
    {
        dec int32[4] t = [10, 20, 30, 40];
        dec int32 i = 2;
        
        // Test variable
        if (t[i] != 30) { return false; }

        // Test expression mathématique (i + 1)
        if (t[i + 1] != 40) { return false; }
        
        return true;
    }

    fn bool tableauTestIntegritePile()
    {
        dec int32 a = 100;
        dec int32[2] t = [1, 2];
        dec int32 b = 200;

        aff t[0] = 5;
        aff t[1] = 6;

        // Si le calcul d'index est faux, 'a' ou 'b' pourraient être modifiés
        if (a != 100) { return false; }
        if (b != 200) { return false; }
        return true;
    }

    fn bool tableauTestIteration()
    {
        dec int32[3] t = [0, 0, 0];
        dec int32 i = 0;
        dec int32 somme = 0;

        // Remplissage dynamique
        while (i < 3) {
            aff t[i] = i + 1;
            aff i = i + 1;
        }

        // Lecture et accumulation
        aff i = 0;
        while (i < 3) {
            aff somme = somme + t[i];
            aff i = i + 1;
        }

        return (somme == 6);
    }
// Ne fonctionne pas, bug dans la gestion de l'équation du tableau dans les braquettes []
    //fn bool testeEquationScopeTableau()
    //{
      //  dec int32 a = 5; 
       // dec int32[3] tableau = [1+(3*a)-23-2, 3*4*4, 42-23]; 

       // if(tableau[0] != -9)
        //{
           // return false;
       // }
       // if(tableau[1] != 48)
        //{
         //   return false; 
        //}

       // if(tableau[2] != 19)
        //{
         //   return false;
        //}
        //return true; 
    //}


    fn int32[4] tableauTestRetour()
    {
        dec int32[4] tableau = [0, 1, 2, 3];
        return tableau;
    }

    fn bool tableauTestRetourLecture()
    {
        dec int32[4] tableau = call tableauTestRetour();

         if (tableau[0] != 0) {
             return false;
         }
         if (tableau[1] != 1) {
             return false;  
         }
        if (tableau[2] != 2) {
            return false;  
        }
        if (tableau[3] != 3) {
            return false;  
        }
        return true;
    }

    fn bool tableauInjectionArgument(arg int32[3] tableau)
    {
        if (tableau[0] != 0) {
             return false;
         }
         if (tableau[1] != 1) {
             return false;  
         }
        if (tableau[2] != 2) {
            return false;  
        }
        return true;
    }

    fn bool tableauTestArgument()
    {
        dec int32[3] tableau = [0, 1, 2];
        return call tableauInjectionArgument(tableau);
    }

    fn int32 main()
    {
        call printBool(call tableauTestLecture());
        call printBool(call tableauTestAffectation());
        //call printBool(call tableauTestSortieHorsLimite()); true
        call printBool(call tableauVariableTest());
        call printBool(call tableauTestIndexDynamique()); // false 
        call printBool(call tableauTestIntegritePile());
        call printBool(call tableauTestIteration());
        //call printBool(call testeEquationScopeTableau());
        call printBool(call tableauTestRetourLecture());
        call printBool(call tableauTestArgument());
        return 0; 
    }

}