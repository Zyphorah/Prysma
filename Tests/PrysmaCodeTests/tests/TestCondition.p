scope {

    // 1. Test de base : Supériorité Entier
    fn bool testIntSup()
    {
        dec int a = 5; 
        if (a > 3) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 2. Test de base : Infériorité Float
    fn bool testFloatInf()
    {
        dec float a = 2.5;
        if (a < 5.0) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 3. Test d'Égalité Entier
    fn bool testIntEgal()
    {
        dec int a = 10;
        if (a == 10) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 4. Test d'Égalité Float (Vérifie la précision)
    fn bool testFloatEgal()
    {
        dec float a = 25.5;
        if (a == 25.5) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 5. Test Logique ET (&&) sans parenthèses externes
    fn bool testAndSimple()
    {
        dec int a = 1;
        dec int b = 1;
        
        if (a == 1 && b == 1) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 6. Test Logique OU (||)
    fn bool testOrSimple()
    {
        dec int a = 0;
        dec int b = 1;
        
        if (a == 1 || b == 1) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 7. Test Complexe : Parenthèses imbriquées (Le cas qui plantait)
    // Vérifie si le parser gère ((A) && (B))
    fn bool testParentheses()
    {
        dec int a = 10;
        dec float b = 25.5;

        if ((a == 10) && (b == 25.5)) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 8. Test Condition sur Arguments (Vérifie la portée des args dans le if)
    fn bool testArgDansIf(arg int val)
    {
        if (val == 100) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 9. Test If Imbriqué (Nested)
    fn bool testImbrique()
    {
        dec int x = 50;
        
        if (x > 10) {
            if (x < 100) {
                return true;
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
        call printBool(call testIntSup());      // true
        call printBool(call testFloatInf());    // true
        call printBool(call testIntEgal());     // true
        call printBool(call testFloatEgal());   // true
        
        call printBool(call testAndSimple());   // true
        call printBool(call testOrSimple());    // true
        
        call printBool(call testParentheses()); // true 
        
        dec int param = 100;
        call printBool(call testArgDansIf(param)); // false

        call printBool(call testImbrique());    // true
    
        return 1; 
    }
}