fn bool testEgaliteInteger()
{
    dec string[] msg = "8.testEgaliteInteger : ";
    call print(ref msg);
    dec bool r = call estEgalInt(42, 42);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testInegaliteInteger()
{
    dec string[] msg = "9.testInegaliteInteger : ";
    call print(ref msg);
    dec bool r = call estEgalInt(10, 20);
    if (r == false) {
        return true;
    }
    return false;
}

fn bool testPlusGrandInteger()
{
    dec string[] msg = "10.testPlusGrandInteger : ";
    call print(ref msg);
    dec bool r = call estPlusGrandInt(100, 50);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testPlusPetitInteger()
{
    dec string[] msg = "11.testPlusPetitInteger : ";
    call print(ref msg);
    dec bool r = call estPlusPetitInt(3, 99);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testValeurAbsolue()
{
    dec string[] msg = "12.testValeurAbsolue : ";
    call print(ref msg);
    dec int32 neg = 0 - 42;
    dec int32 r = call valeurAbsolue(neg);
    if (r == 42) {
        return true;
    }
    return false;
}

fn bool testMaximum()
{
    dec string[] msg = "13.testMaximum : ";
    call print(ref msg);
    dec int32 r = call maximum(15, 30);
    if (r == 30) {
        return true;
    }
    return false;
}

fn bool testMinimum()
{
    dec string[] msg = "14.testMinimum : ";
    call print(ref msg);
    dec int32 r = call minimum(15, 30);
    if (r == 15) {
        return true;
    }
    return false;
}

fn void executerTestsInteger()
{
    call print(call testEgaliteInteger()); call backSlashN();
    call print(call testInegaliteInteger()); call backSlashN();
    call print(call testPlusGrandInteger()); call backSlashN();
    call print(call testPlusPetitInteger()); call backSlashN();
    call print(call testValeurAbsolue()); call backSlashN();
    call print(call testMaximum()); call backSlashN();
    call print(call testMinimum()); call backSlashN();
}
