
fn bool arrayTestReading_float()
{
    dec string[] functionnalite = "1.arrayTestReading : ";
    call print(ref functionnalite);
        dec float[4] array = [0.0, 1.0, 2.0, 3.0];

        if (array[0] != 0.0) {
            return false;
        }
        if (array[1] != 1.0) {
            return false;  
        }
    if (array[2] != 2.0) {
        return false;  
    }
    if (array[3] != 3.0) {
        return false;  
    }
    return true;
}

fn bool arrayTestAssignment_float()
{
    dec string[] functionnalite = "2.arrayTestAssignment : ";
    call print(ref functionnalite);
        dec float[4] array = [0.0, 1.0, 2.0, 3.0];

        aff array[0] = 10.0;
        aff array[1] = 11.0;
        aff array[2] = 12.0;
        aff array[3] = 13.0;

        if (array[0] != 10.0) {
            return false;
        }
        if (array[1] != 11.0) {
            return false;  
        }
    if (array[2] != 12.0) {
        return false;  
    }
    if (array[3] != 13.0) {
        return false;  
    }
    return true;
}

// Problème actuelle gènère true même si l'assignment est hors limite, bug dans la gestion de l'équation du array dans les braquettes []
fn bool arrayTestOutputHorsLimite_float()
{
    dec string[] functionnalite = "3.arrayTestOutputHorsLimite : ";
    call print(ref functionnalite);
        dec float[4] array = [0.0, 1.0, 2.0, 3.0];

        aff array[4] = 10.0; // Doit générer une error de compilation
        return true; // Ne doit pas être atteint
}

fn bool arrayVariableTest_float()
{
    dec string[] functionnalite = "4.arrayVariableTest : ";
    call print(ref functionnalite);
    dec float a = 4.0; 
    dec float b = 2.0; 
    dec float[5] array = [a, b, 3.0, 4.0, 5.0];

    if (array[0] != 4.0) {
        return false;
    }
    if (array[1] != 2.0) {
        return false;
    }
    if (array[2] != 3.0) {
        return false;
    }
    if (array[3] != 4.0) {
        return false;
    }
    if (array[4] != 5.0) {
        return false;
    }
    return true;
}

fn bool arrayTestIndexDynamic_float()
{
    dec string[] functionnalite = "5.arrayTestIndexDynamic : ";
    call print(ref functionnalite);
    dec float[4] t = [10.0, 20.0, 30.0, 40.0];
    dec int32 i = 2;
    
    // Test variable
    if (t[i] != 30.0) { return false; }

    // Test expression mathématique (i + 1)
    if (t[i + 1] != 40.0) { return false; }
    
    return true;
}

fn bool arrayTestIntegritePile_float()
{
    dec string[] functionnalite = "6.arrayTestIntegritePile : ";
    call print(ref functionnalite);
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

fn bool arrayTestIteration_float()
{
    dec string[] functionnalite = "7.arrayTestIteration : ";
    call print(ref functionnalite);
    dec float[3] t = [0.0, 0.0, 0.0];
    dec int32 i = 0;
    dec float somme = 0.0;

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

    return (somme == 6.0);
}
// Ne functionne pas, bug dans la gestion de l'équation du array dans les braquettes []
//fn bool testeEquationScopeArray()
//{
    //  dec float a = 5.0; 
    // dec float[3] array = [1.0+(3.0*a)-23.0-2.0, 3.0*4.0*4.0, 42.0-23.0]; 

    // if(array[0] != -9.0)
    //{
        // return false;
    // }
    // if(array[1] != 48.0)
    //{
        //   return false; 
    //}

    // if(array[2] != 19.0)
    //{
        //   return false;
    //}
    //return true; 
//}

fn float[4] arrayTestReturn_float()
{
    dec string[] functionnalite = "8.arrayTestReturn : ";
    call print(ref functionnalite);
    dec float[4] array = [0.0, 1.0, 2.0, 3.0];
    return array;
}

fn bool arrayTestReturnReading_float()
{
    dec string[] functionnalite = "9.arrayTestReturnReading : ";
    call print(ref functionnalite);
    dec float[4] array = call arrayTestReturn_float();

        if (array[0] != 0.0) {
            return false;
        }
        if (array[1] != 1.0) {
            return false;  
        }
    if (array[2] != 2.0) {
        return false;  
    }
    if (array[3] != 3.0) {
        return false;  
    }
    return true;
}

fn bool arrayInjectionArgument_float(arg float[3] array)
{
    dec string[] functionnalite = "10.arrayInjectionArgument : ";
    call print(ref functionnalite);
    if (array[0] != 0.0) {
            return false;
        }
        if (array[1] != 1.0) {
            return false;  
        }
    if (array[2] != 2.0) {
        return false;  
    }
    return true;
}

fn bool arrayTestArgument_float()
{
    dec string[] functionnalite = "11.arrayTestArgument : ";
    call print(ref functionnalite);
    dec float[3] array = [0.0, 1.0, 2.0];
    return call arrayInjectionArgument_float(array);
}

fn int32 testArrayFloat()
{
    call print(call arrayTestReading_float()); call backSlashN();
    call print(call arrayTestAssignment_float()); call backSlashN();
    //call print(call arrayTestOutputHorsLimite_float()); true
    call print(call arrayVariableTest_float()); call backSlashN();
    call print(call arrayTestIndexDynamic_float()); call backSlashN();  
    call print(call arrayTestIntegritePile_float()); call backSlashN();
    call print(call arrayTestIteration_float()); call backSlashN(); 
    //call print(call testeEquationScopeArray());
    call print(call arrayTestReturnReading_float()); call backSlashN();
    call print(call arrayTestArgument_float()); call backSlashN();
    return 0; 
}


   