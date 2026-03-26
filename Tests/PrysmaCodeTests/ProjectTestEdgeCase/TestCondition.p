// 1. Basic Test: Integer Greater Than
fn bool testIntGreater()
{
    dec string[] feature = "1.testIntGreater : ";
    call print(ref feature);
    dec int32 a = 5; 
    if (a > 3) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 2. Basic Test: Float Less Than
fn bool testFloatLess()
{
    dec string[] feature = "2.testFloatLess : ";
    call print(ref feature);
    dec float a = 2.5;
    if (a < 5.0) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 3. Integer Equality Test
fn bool testIntEqual()
{
    dec string[] feature = "3.testIntEqual : ";
    call print(ref feature);
    dec int32 a = 10;
    if (a == 10) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 4. Float Equality Test (Checks precision)
fn bool testFloatEqual()
{
    dec string[] feature = "4.testFloatEqual : ";
    call print(ref feature);
    dec float a = 25.5;
    if (a == 25.5) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 5. Logical AND (&&) Test without external parentheses
fn bool testAndSimple()
{
    dec string[] feature = "5.testAndSimple : ";
    call print(ref feature);
    dec int32 a = 1;
    dec int32 b = 1;
    
    if (a == 1 && b == 1) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 6. Logical OR (||) Test
fn bool testOrSimple()
{
    dec string[] feature = "6.testOrSimple : ";
    call print(ref feature);
    dec int32 a = 0;
    dec int32 b = 1;
    
    if (a == 1 || b == 1) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 7. Complex Test: Nested Parentheses (The case that failed)
// Checks if the parser handles ((A) && (B))
fn bool testParentheses()
{
    dec string[] feature = "7.testParentheses : ";
    call print(ref feature);
    dec int32 a = 10;
    dec float b = 25.5;

    if ((a == 10) && (b == 25.5)) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 8. Test condition on Arguments (Checks argument scope in if)
fn bool testArgInIf(arg int32 value)
{
    dec string[] feature = "8.testArgInIf : ";
    call print(ref feature);
    if (value == 100) {
        return true;
    } else {
        return false;
    }
    return false;
}

// 9. Nested If Test
fn bool testNestedIf()
{
    dec string[] feature = "9.testNestedIf : ";
    call print(ref feature);
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

fn int32 testCondition()
{
    call print(call testIntGreater());   call backSlashN();   // true 
    call print(call testFloatLess());    call backSlashN(); // true
    call print(call testIntEqual());     call backSlashN(); // true
    call print(call testFloatEqual());   call backSlashN(); // true
    
    call print(call testAndSimple());   call backSlashN(); // true
    call print(call testOrSimple());    call backSlashN(); // true
    
    call print(call testParentheses()); call backSlashN(); // true 
    
    dec int32 param = 100;
    call print(call testArgInIf(param)); call backSlashN(); // true

    call print(call testNestedIf());    call backSlashN(); // true

    return 1; 
}

