scope
{
    fn bool tableauTestLecture()
    {
         dec float[4] tableau = [0.0, 1.0, 2.0, 3.0];

         if (tableau[0] != 0.0) {
             return false;
         }
         if (tableau[1] != 1.0) {
             return false;  
         }
        if (tableau[2] != 2.0) {
            return false;  
        }
        if (tableau[3] != 3.0) {
            return false;  
        }
        return true;
    }

    fn bool tableauTestAffectation()
    {
         dec float[4] tableau = [0.0, 1.0, 2.0, 3.0];

         aff tableau[0] = 10.0;
         aff tableau[1] = 11.0;
         aff tableau[2] = 12.0;
         aff tableau[3] = 13.0;

         if (tableau[0] != 10.0) {
             return false;
         }
         if (tableau[1] != 11.0) {
             return false;  
         }
        if (tableau[2] != 12.0) {
            return false;  
        }
        if (tableau[3] != 13.0) {
            return false;  
        }
        return true;
    }

    fn bool tableauTestSortieHorsLimite()
    {
         dec float[4] tableau = [0.0, 1.0, 2.0, 3.0];

         aff tableau[4] = 10.0; // Doit générer une erreur de compilation
         return true; // Ne doit pas être atteint
    }

    fn bool tableauVariableTest()
    {
        dec float a = 4.0; 
        dec float b = 2.0; 
        dec float[5] tableau = [a, b, 3.0, 4.0, 5.0];

        if (tableau[0] != 4.0) {
            return false;
        }
        if (tableau[1] != 2.0) {
            return false;
        }
        if (tableau[2] != 3.0) {
            return false;
        }
        if (tableau[3] != 4.0) {
            return false;
        }
        if (tableau[4] != 5.0) {
            return false;
        }
        return true;
    }
    
    fn bool tableauTestIndexDynamique()
    {
        dec float[4] t = [10.0, 20.0, 30.0, 40.0];
        dec int32 i = 2;
        
        // Test variable
        if (t[i] != 30.0) { return false; }

        // Test expression mathématique (i + 1)
        if (t[i + 1] != 40.0) { return false; }
        
        return true;
    }

    fn bool tableauTestIntegritePile()
    {
        dec float a = 100.0;
        dec float[2] t = [1.0, 2.0];
        dec float b = 200.0;

        aff t[0] = 5.0;
        aff t[1] = 6.0;

        // Si le calcul d'index est faux, 'a' ou 'b' pourraient être modifiés
        if (a != 100.0) { return false; }
        if (b != 200.0) { return false; }
        return true;
    }

    fn bool tableauTestIteration()
    {
        dec float[3] t = [0.0, 0.0, 0.0];
        dec int32 i = 0;
        dec float somme = 0.0;

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

        return (somme == 6.0);
    }
// Ne fonctionne pas, bug dans la gestion de l'équation du tableau dans les braquettes []
    //fn bool testeEquationScopeTableau()
    //{
      //  dec float a = 5.0; 
       // dec float[3] tableau = [1.0+(3.0*a)-23.0-2.0, 3.0*4.0*4.0, 42.0-23.0]; 

       // if(tableau[0] != -9.0)
        //{
           // return false;
       // }
       // if(tableau[1] != 48.0)
        //{
         //   return false; 
        //}

       // if(tableau[2] != 19.0)
        //{
         //   return false;
        //}
        //return true; 
    //}

    fn float[4] tableauTestRetour()
    {
        dec float[4] tableau = [0.0, 1.0, 2.0, 3.0];
        return tableau;
    }

    fn bool tableauTestRetourLecture()
    {
        dec float[4] tableau = call tableauTestRetour();

         if (tableau[0] != 0.0) {
             return false;
         }
         if (tableau[1] != 1.0) {
             return false;  
         }
        if (tableau[2] != 2.0) {
            return false;  
        }
        if (tableau[3] != 3.0) {
            return false;  
        }
        return true;
    }

    fn bool tableauInjectionArgument(arg float[3] tableau)
    {
        if (tableau[0] != 0.0) {
             return false;
         }
         if (tableau[1] != 1.0) {
             return false;  
         }
        if (tableau[2] != 2.0) {
            return false;  
        }
        return true;
    }

    fn bool tableauTestArgument()
    {
        dec float[3] tableau = [0.0, 1.0, 2.0];
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
        call printBool(call tableauTestIteration()); // false
        //call printBool(call testeEquationScopeTableau());
        call printBool(call tableauTestRetourLecture());
        call printBool(call tableauTestArgument());

        return 0; 
    }

}

   