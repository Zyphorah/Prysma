fn bool arrayTestReading_int()
{
    dec string[] feature = "1.arrayTestReading : ";
    call print(ref feature);
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
    dec string[] feature = "2.arrayTestAssignment : ";
    call print(ref feature);
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

fn bool arrayTestOutOfBounds_int()
{
    dec string[] feature = "3.arrayTestOutOfBounds : ";
    call print(ref feature);
    dec int32[4] array = [0, 1, 2, 3];

    aff array[4] = 10; // Must generate a compilation error
    return true; // Must not be reached
}

fn bool arrayVariableTest_int()
{
    dec string[] feature = "4.arrayVariableTest : ";
    call print(ref feature);
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

fn bool arrayTestDynamicIndex_int()
{
    dec string[] feature = "5.arrayTestDynamicIndex : ";
    call print(ref feature);
    dec int32[4] t = [10, 20, 30, 40];
    dec int32 i = 2;
    
    // Test variable
    if (t[i] != 30) { return false; }

    // Test mathematical expression (i + 1)
    if (t[i + 1] != 40) { return false; }
    
    return true;
}

fn bool arrayTestStackIntegrity_int()
{
    dec string[] feature = "6.arrayTestStackIntegrity : ";
    call print(ref feature);
    dec int32 a = 100;
    dec int32[2] t = [1, 2];
    dec int32 b = 200;

    aff t[0] = 5;
    aff t[1] = 6;

    // If the index calculation is wrong, 'a' or 'b' could be modified
    if (a != 100) { return false; }
    if (b != 200) { return false; }
    return true;
}

fn bool arrayTestIteration_int()
{
    dec string[] feature = "7.arrayTestIteration : ";
    call print(ref feature);
    dec int32[3] t = [0, 0, 0];
    dec int32 i = 0;
    dec int32 sum = 0;

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

    return (sum == 6);
}

// Does not work, bug in handling array equation in brackets []
//fn bool testEquationScopeArray()
//{
//    dec int32 a = 5; 
//    dec int32[3] array = [1+(3*a)-23-2, 3*4*4, 42-23]; 
//
//    if(array[0] != -9)
//    {
//        return false;
//    }
//    if(array[1] != 48)
//    {
//        return false; 
//    }
//
//    if(array[2] != 19)
//    {
//        return false;
//    }
//    return true; 
//}

fn int32[4] arrayTestReturn_int()
{
    dec string[] feature = "8.arrayTestReturn : ";
    call print(ref feature);
    dec int32[4] array = [0, 1, 2, 3];
    return array;
}

fn bool arrayTestReturnReading_int()
{
    dec string[] feature = "9.arrayTestReturnReading : ";
    call print(ref feature);
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
    dec string[] feature = "10.arrayInjectionArgument : ";
    call print(ref feature);
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
    dec string[] feature = "11.arrayTestArgument : ";
    call print(ref feature);
    dec int32[3] array = [0, 1, 2];
    return call arrayInjectionArgument_int(array);
}

fn int32 testArrayInt()
{   
    call print(call arrayTestReading_int());  call backSlashN();
    call print(call arrayTestAssignment_int());  call backSlashN();
    //call print(call arrayTestOutOfBounds_int()); // true
    call print(call arrayVariableTest_int());  call backSlashN();
    call print(call arrayTestDynamicIndex_int());  call backSlashN(); 
    call print(call arrayTestStackIntegrity_int());  call backSlashN();
    call print(call arrayTestIteration_int());  call backSlashN();
    //call print(call testEquationScopeArray());
    call print(call arrayTestReturnReading_int());  call backSlashN();
    call print(call arrayTestArgument_int());  call backSlashN();
    return 0; 
}

