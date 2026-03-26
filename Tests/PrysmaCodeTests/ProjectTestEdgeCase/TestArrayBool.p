fn bool arrayTestReading_bool()
{
    dec string[] feature = "1.arrayTestReading : ";
    call print(ref feature);
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
    dec string[] feature = "2.arrayTestAssignment : ";
    call print(ref feature);
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

fn bool arrayTestOutOfBounds_bool()
{
    dec string[] feature = "3.arrayTestOutOfBounds : ";
    call print(ref feature);
    dec bool[4] array = [true, true, true, true];

    aff array[4] = false; // Must generate a compilation error
    return true; // Must not be reached
}

fn bool arrayVariableTest_bool()
{
    dec string[] feature = "4.arrayVariableTest : ";
    call print(ref feature);
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

// Bug to fix
// The compiler generates incorrect LLVM code for array access with an expression as index (e.g. t[i + 1]).
// Instead of first calculating the index (i + 1), it seems to load the value of t[i] (a boolean)
// then tries to add 1 to this boolean, which causes a type error in LLVM (add i1, i32).
// The code generation logic for array index equations must be fixed.
//fn bool arrayTestDynamicIndex()
//{
//    dec bool[4] t = [true, false, true, false];
//    dec int32 i = 2;
//    
//    // Test variable
//    if (t[i] != true) { return false; }
//
//    // Test mathematical expression (i + 1)
//    if (t[i + 1] != false) { return false; }
//    
//    return true;
//}

fn bool arrayTestStackIntegrity_bool()
{
    dec string[] feature = "5.arrayTestStackIntegrity : ";
    call print(ref feature);
    dec bool a = true;
    dec bool[2] t = [false, false];
    dec bool b = false;

    aff t[0] = true;
    aff t[1] = true;

    // If the index calculation is wrong, 'a' or 'b' could be modified
    if (a != true) { return false; }
    if (b != false) { return false; }
    return true;
}

fn bool arrayTestIteration_bool()
{
    dec string[] feature = "6.arrayTestIteration : ";
    call print(ref feature);
    dec bool[3] t = [false, false, false];
    dec int32 i = 0;

    // Dynamic filling
    while (i < 3) {
        aff t[i] = true;
        aff i = i + 1;
    }

    // Reading and accumulation
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
    dec string[] feature = "7.arrayTestReturn : ";
    call print(ref feature);
    dec bool[4] array = [true, false, true, false];
    return array;
}

fn bool arrayTestReturnReading_bool()
{
    dec string[] feature = "8.arrayTestReturnReading : ";
    call print(ref feature);
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
    dec string[] feature = "9.arrayInjectionArgument : ";
    call print(ref feature);
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
    dec string[] feature = "10.arrayTestArgument : ";
    call print(ref feature);
    dec bool[3] array = [true, false, true];
    return call arrayInjectionArgument_bool(array);
}

fn int32 testArrayBool()
{
    call print(call arrayTestReading_bool()); call backSlashN();
    call print(call arrayTestAssignment_bool()); call backSlashN();
    //call print(call arrayTestOutOfBounds_bool()); // true
    call print(call arrayVariableTest_bool()); call backSlashN();
    //call print(call arrayTestDynamicIndex()); // false
    call print(call arrayTestStackIntegrity_bool()); call backSlashN();
    call print(call arrayTestIteration_bool()); call backSlashN();
    call print(call arrayTestReturnReading_bool()); call backSlashN();
    call print(call arrayTestArgument_bool()); call backSlashN();

    return 0; 
}
