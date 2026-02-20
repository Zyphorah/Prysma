scope {

    // 1. Test de base : Supériorité Entier
    fn bool testIntSup()
    {
        dec string[] fonctionnalite = "1.testIntSup : ";
        call printString(ref fonctionnalite);
        dec int32 a = 5; 
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
        dec string[] fonctionnalite = "2.testFloatInf : ";
        call printString(ref fonctionnalite);
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
        dec string[] fonctionnalite = "3.testIntEgal : ";
        call printString(ref fonctionnalite);
        dec int32 a = 10;
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
        dec string[] fonctionnalite = "4.testFloatEgal : ";
        call printString(ref fonctionnalite);
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
        dec string[] fonctionnalite = "5.testAndSimple : ";
        call printString(ref fonctionnalite);
        dec int32 a = 1;
        dec int32 b = 1;
        
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
        dec string[] fonctionnalite = "6.testOrSimple : ";
        call printString(ref fonctionnalite);
        dec int32 a = 0;
        dec int32 b = 1;
        
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
        dec string[] fonctionnalite = "7.testParentheses : ";
        call printString(ref fonctionnalite);
        dec int32 a = 10;
        dec float b = 25.5;

        if ((a == 10) && (b == 25.5)) {
            return true;
        } else {
            return false;
        }
        return false;
    }

    // 8. Test Condition sur Arguments (Vérifie la portée des args dans le if)
    fn bool testArgDansIf(arg int32 val)
    {
        dec string[] fonctionnalite = "8.testArgDansIf : ";
        call printString(ref fonctionnalite);
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
        dec string[] fonctionnalite = "9.testImbrique : ";
        call printString(ref fonctionnalite);
        dec int32 x = 50;
        
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

    fn int32 main()
    {
        call printBool(call testIntSup());   call backSlashN();   // true 
        call printBool(call testFloatInf());    call backSlashN(); // true
        call printBool(call testIntEgal());     call backSlashN(); // true
        call printBool(call testFloatEgal());   call backSlashN(); // true
        
        call printBool(call testAndSimple());   call backSlashN(); // true
        call printBool(call testOrSimple());    call backSlashN(); // true
        
        call printBool(call testParentheses()); call backSlashN(); // true 
        
        dec int32 param = 100;
        call printBool(call testArgDansIf(param)); call backSlashN(); // true

        call printBool(call testImbrique());    call backSlashN(); // true
    
        return 1; 
    }
}
