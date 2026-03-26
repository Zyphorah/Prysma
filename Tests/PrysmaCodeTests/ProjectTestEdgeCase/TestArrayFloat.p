fn bool arrayTestReading_float()
{
    dec string[] feature = "1.arrayTestReading : ";
    call print(ref feature);
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
    dec string[] feature = "2.arrayTestAssignment : ";
    call print(ref feature);
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

// Current problem: generates true even if the assignment is out of bounds, bug in the management of the equation of the array in the brackets []
fn bool arrayTestOutOfBounds_float()
{
    dec string[] feature = "3.arrayTestOutOfBounds : ";
    call print(ref feature);
    dec float[4] array = [0.0, 1.0, 2.0, 3.0];

    aff array[4] = 10.0; // Must generate a compilation error
    return true; // Must not be reached
}

fn bool arrayVariableTest_float()
{
    dec string[] feature = "4.arrayVariableTest : ";
    call print(ref feature);
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

fn bool arrayTestDynamicIndex_float()
{
    dec string[] feature = "5.arrayTestDynamicIndex : ";
    call print(ref feature);
    dec float[4] t = [10.0, 20.0, 30.0, 40.0];
    dec int32 i = 2;
    
    // Test variable
    if (t[i] != 30.0) { return false; }

    // Test mathematical expression (i + 1)
    if (t[i + 1] != 40.0) { return false; }
    
    return true;
}

fn bool arrayTestStackIntegrity_float()
{
    dec string[] feature = "6.arrayTestStackIntegrity : ";
    call print(ref feature);
    dec float a = 100.0;
    dec float[2] t = [1.0, 2.0];
    dec float b = 200.0;

    aff t[0] = 5.0;
    aff t[1] = 6.0;

    // If the index calculation is wrong, 'a' or 'b' could be modified
    if (a != 100.0) { return false; }
    if (b != 200.0) { return false; }
    return true;
}

fn bool arrayTestIteration_float()
{
    dec string[] feature = "7.arrayTestIteration : ";
    call print(ref feature);
    dec float[3] t = [0.0, 0.0, 0.0];
    dec int32 i = 0;
    dec float sum = 0.0;

    // Dynamic filling
    while (i < 3) {
        aff t[i] = i + 1;
        aff i = i + 1;
    }

    // Reading and accumulation
    aff i = 0;
    while (i < 3) {
        aff sum = sum + t[i];
        aff i = i + 1;
    }

    return (sum == 6.0);
}
// Does not work, bug in handling array equation in brackets []
//fn bool testEquationScopeArray()
//{
//    dec float a = 5.0; 
//    dec float[3] array = [1.0+(3.0*a)-23.0-2.0, 3.0*4.0*4.0, 42.0-23.0]; 
//
//    if(array[0] != -9.0)
//    {
//        return false;
//    }
//    if(array[1] != 48.0)
//    {
//        return false; 
//    }
//
//    if(array[2] != 19.0)
//    {
//        return false;
//    }
//    return true; 
//}

fn float[4] arrayTestReturn_float()
{
    dec string[] feature = "8.arrayTestReturn : ";
    call print(ref feature);
    dec float[4] array = [0.0, 1.0, 2.0, 3.0];
    return array;
}

fn bool arrayTestReturnReading_float()
{
    dec string[] feature = "9.arrayTestReturnReading : ";
    call print(ref feature);
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
    dec string[] feature = "10.arrayInjectionArgument : ";
    call print(ref feature);
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
    dec string[] feature = "11.arrayTestArgument : ";
    call print(ref feature);
    dec float[3] array = [0.0, 1.0, 2.0];
    return call arrayInjectionArgument_float(array);
}

fn int32 testArrayFloat()
{
    call print(call arrayTestReading_float()); call backSlashN();
    call print(call arrayTestAssignment_float()); call backSlashN();
    //call print(call arrayTestOutOfBounds_float()); // true
    call print(call arrayVariableTest_float()); call backSlashN();
    call print(call arrayTestDynamicIndex_float()); call backSlashN();  
    call print(call arrayTestStackIntegrity_float()); call backSlashN();
    call print(call arrayTestIteration_float()); call backSlashN(); 
    //call print(call testEquationScopeArray());
    call print(call arrayTestReturnReading_float()); call backSlashN();
    call print(call arrayTestArgument_float()); call backSlashN();
    return 0; 
}


