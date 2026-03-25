fn bool arrayTestReading_int()
{
    dec string[] functionnalite = "1.arrayTestReading : ";
    call print(ref functionnalite);

        dec int32[4] array = [0, 1, 2, 3];

        if (array[0] != 0) {
            return false;
        }
        if (array[1] != 1) {
            return false;  
        }
    if (array[2] != 2) {
        return false;  
    }
    if (array[3] != 3) {
        return false;  
    }
    return true;
}

fn bool arrayTestAssignment_int()
{
    dec string[] functionnalite = "2.arrayTestAssignment : ";
    call print(ref functionnalite);

        dec int32[4] array = [0, 1, 2, 3];

        aff array[0] = 10;
        aff array[1] = 11;
        aff array[2] = 12;
        aff array[3] = 13;

        if (array[0] != 10) {
            return false;
        }
        if (array[1] != 11) {
            return false;  
        }
    if (array[2] != 12) {
        return false;  
    }
    if (array[3] != 13) {
        return false;  
    }
    return true;
}

fn bool arrayTestOutputHorsLimite_int()
{
    dec string[] functionnalite = "3.arrayTestOutputHorsLimite : ";
    call print(ref functionnalite);

        dec int32[4] array = [0, 1, 2, 3];

        aff array[4] = 10; // Doit générer une error de compilation
        return true; // Ne doit pas être atteint
}

fn bool arrayVariableTest_int()
{
    dec string[] functionnalite = "4.arrayVariableTest : ";
    call print(ref functionnalite);

    dec int32 a = 4; 
    dec int32 b = 2; 
    dec int32[5] array = [a, b, 3, 4, 5];

    if (array[0] != 4) {
        return false;
    }
    if (array[1] != 2) {
        return false;
    }
    if (array[2] != 3) {
        return false;
    }
    if (array[3] != 4) {
        return false;
    }
    if (array[4] != 5) {
        return false;
    }
    return true;
}

fn bool arrayTestIndexDynamic_int()
{
    dec string[] functionnalite = "5.arrayTestIndexDynamic : ";
    call print(ref functionnalite);

    dec int32[4] t = [10, 20, 30, 40];
    dec int32 i = 2;
    
    // Test variable
    if (t[i] != 30) { return false; }

    // Test expression mathématique (i + 1)
    if (t[i + 1] != 40) { return false; }
    
    return true;
}

fn bool arrayTestIntegritePile_int()
{
    dec string[] functionnalite = "6.arrayTestIntegritePile : ";
    call print(ref functionnalite);

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

fn bool arrayTestIteration_int()
{
    dec string[] functionnalite = "7.arrayTestIteration : ";
    call print(ref functionnalite);

    dec int32[3] t = [0, 0, 0];
    dec int32 i = 0;
    dec int32 somme = 0;

    // Filling dynamique
    while (i < 3) {
        aff t[i] = i + 1;
        aff i = i + 1;
    }

    // Reading et accumulation
    aff i = 0;
    while (i < 3) {
        aff somme = somme + t[i];
        aff i = i + 1;
    }

    return (somme == 6);
}

// Ne functionne pas, bug dans la gestion de l'équation du array dans les braquettes []
//fn bool testeEquationScopeArray()
//{
    //  dec int32 a = 5; 
    // dec int32[3] array = [1+(3*a)-23-2, 3*4*4, 42-23]; 

    // if(array[0] != -9)
    //{
        // return false;
    // }
    // if(array[1] != 48)
    //{
        //   return false; 
    //}

    // if(array[2] != 19)
    //{
        //   return false;
    //}
    //return true; 
//}


fn int32[4] arrayTestReturn_int()
{
    dec string[] functionnalite = "8.arrayTestReturn : ";
    call print(ref functionnalite);

    dec int32[4] array = [0, 1, 2, 3];
    return array;
}

fn bool arrayTestReturnReading_int()
{
    dec string[] functionnalite = "9.arrayTestReturnReading : ";
    call print(ref functionnalite);

    dec int32[4] array = call arrayTestReturn_int();

        if (array[0] != 0) {
            return false;
        }
        if (array[1] != 1) {
            return false;  
        }
    if (array[2] != 2) {
        return false;  
    }
    if (array[3] != 3) {
        return false;  
    }
    return true;
}

fn bool arrayInjectionArgument_int(arg int32[3] array)
{
    dec string[] functionnalite = "10.arrayInjectionArgument : ";
    call print(ref functionnalite);

    if (array[0] != 0) {
            return false;
        }
        if (array[1] != 1) {
            return false;  
        }
    if (array[2] != 2) {
        return false;  
    }
    return true;
}

fn bool arrayTestArgument_int()
{
    dec string[] functionnalite = "11.arrayTestArgument : ";
    call print(ref functionnalite);

    dec int32[3] array = [0, 1, 2];
    return call arrayInjectionArgument_int(array);
}

fn int32 testArrayInt()
{   
    call print(call arrayTestReading_int());  call backSlashN();
    call print(call arrayTestAssignment_int());  call backSlashN();
    //call print(call arrayTestOutputHorsLimite_int()); true
    call print(call arrayVariableTest_int());  call backSlashN();
    call print(call arrayTestIndexDynamic_int());  call backSlashN(); 
    call print(call arrayTestIntegritePile_int());  call backSlashN();
    call print(call arrayTestIteration_int());  call backSlashN();
    //call print(call testeEquationScopeArray());
    call print(call arrayTestReturnReading_int());  call backSlashN();
    call print(call arrayTestArgument_int());  call backSlashN();
    return 0; 
}

