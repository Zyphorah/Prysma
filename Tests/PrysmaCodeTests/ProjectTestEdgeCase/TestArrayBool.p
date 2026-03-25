
fn bool arrayTestReading_bool()
{
    dec string[] functionnalite = "1.arrayTestReading : ";
    call print(ref functionnalite);
        dec bool[4] array = [true, false, true, false];

        if (array[0] != true) {
            return false;
        }
        if (array[1] != false) {
            return false;  
        }
    if (array[2] != true) {
        return false;  
    }
    if (array[3] != false) {
        return false;  
    }
    return true;
}

fn bool arrayTestAssignment_bool()
{
    dec string[] functionnalite = "2.arrayTestAssignment : ";
    call print(ref functionnalite);
        dec bool[4] array = [false, false, false, false];

        aff array[0] = true;
        aff array[1] = true;
        aff array[2] = false;
        aff array[3] = true;

        if (array[0] != true) {
            return false;
        }
        if (array[1] != true) {
            return false;  
        }
    if (array[2] != false) {
        return false;  
    }
    if (array[3] != true) {
        return false;  
    }
    return true;
}

fn bool arrayTestOutputHorsLimite_bool()
{
    dec string[] functionnalite = "3.arrayTestOutputHorsLimite : ";
    call print(ref functionnalite);
        dec bool[4] array = [true, true, true, true];

        aff array[4] = false; // Doit générer une error de compilation
        return true; // Ne doit pas être atteint
}

fn bool arrayVariableTest_bool()
{
    dec string[] functionnalite = "4.arrayVariableTest : ";
    call print(ref functionnalite);
    dec bool a = true; 
    dec bool b = false; 
    dec bool[5] array = [a, b, true, false, true];

    if (array[0] != true) {
        return false;
    }
    if (array[1] != false) {
        return false;
    }
    if (array[2] != true) {
        return false;
    }
    if (array[3] != false) {
        return false;
    }
    if (array[4] != true) {
        return false;
    }
    return true;
}

// bug à corriger
// Le compiler génère un code LLVM incorrect pour l'accès à un array avec une expression comme index (ex: t[i + 1]).
// Au lieu de calculer d'abord l'index (i + 1), il semble loadr la valeur de t[i] (un booléen)
// puis essaie d'ajouter 1 à ce booléen, ce qui cause une error de type dans LLVM (add i1, i32).
// La logique de génération de code pour les équations d'index de array doit être corrigée.
//fn bool arrayTestIndexDynamic()
//{
//    dec bool[4] t = [true, false, true, false];
//    dec int32 i = 2;
//    
//    // Test variable
//    if (t[i] != true) { return false; }
//
//    // Test expression mathématique (i + 1)
//    if (t[i + 1] != false) { return false; }
//    
//    return true;
//}

fn bool arrayTestIntegritePile_bool()
{
    dec string[] functionnalite = "5.arrayTestIntegritePile : ";
    call print(ref functionnalite);
    dec bool a = true;
    dec bool[2] t = [false, false];
    dec bool b = false;

    aff t[0] = true;
    aff t[1] = true;

    // Si le calcul d'index est faux, 'a' ou 'b' pourraient être modifiés
    if (a != true) { return false; }
    if (b != false) { return false; }
    return true;
}

fn bool arrayTestIteration_bool()
{
    dec string[] functionnalite = "6.arrayTestIteration : ";
    call print(ref functionnalite);
    dec bool[3] t = [false, false, false];
    dec int32 i = 0;

    // Filling dynamique
    while (i < 3) {
        aff t[i] = true;
        aff i = i + 1;
    }

    // Reading et accumulation
    aff i = 0;
    while (i < 3) {
        if(t[i] != true)
        {
            return false;
        }
        aff i = i + 1;
    }

    return true;
}

fn bool[4] arrayTestReturn_bool()
{
    dec string[] functionnalite = "7.arrayTestReturn : ";
    call print(ref functionnalite);
    dec bool[4] array = [true, false, true, false];
    return array;
}

fn bool arrayTestReturnReading_bool()
{
    dec string[] functionnalite = "8.arrayTestReturnReading : ";
    call print(ref functionnalite);
    dec bool[4] array = call arrayTestReturn_bool();

        if (array[0] != true) {
            return false;
        }
        if (array[1] != false) {
            return false;  
        }
    if (array[2] != true) {
        return false;  
    }
    if (array[3] != false) {
        return false;  
    }
    return true;
}

fn bool arrayInjectionArgument_bool(arg bool[3] array)
{
    dec string[] functionnalite = "9.arrayInjectionArgument : ";
    call print(ref functionnalite);
    if (array[0] != true) {
            return false;
        }
        if (array[1] != false) {
            return false;  
        }
    if (array[2] != true) {
        return false;  
    }
    return true;
}

fn bool arrayTestArgument_bool()
{
    dec string[] functionnalite = "10.arrayTestArgument : ";
    call print(ref functionnalite);
    dec bool[3] array = [true, false, true];
    return call arrayInjectionArgument_bool(array);
}

fn int32 testArrayBool()
{
    call print(call arrayTestReading_bool()); call backSlashN();
    call print(call arrayTestAssignment_bool()); call backSlashN();
    //call print(call arrayTestOutputHorsLimite_bool()); true
    call print(call arrayVariableTest_bool()); call backSlashN();
    //call print(call arrayTestIndexDynamic()); // false
    call print(call arrayTestIntegritePile_bool()); call backSlashN();
    call print(call arrayTestIteration_bool()); call backSlashN();
    call print(call arrayTestReturnReading_bool()); call backSlashN();
    call print(call arrayTestArgument_bool()); call backSlashN();

    return 0; 
}
