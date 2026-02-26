fn bool testEgaliteEntier()
{
    dec string[] msg = "8.testEgaliteEntier : ";
    call print(ref msg);
    dec bool r = call estEgalInt(42, 42);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testInegaliteEntier()
{
    dec string[] msg = "9.testInegaliteEntier : ";
    call print(ref msg);
    dec bool r = call estEgalInt(10, 20);
    if (r == false) {
        return true;
    }
    return false;
}

fn bool testPlusGrandEntier()
{
    dec string[] msg = "10.testPlusGrandEntier : ";
    call print(ref msg);
    dec bool r = call estPlusGrandInt(100, 50);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testPlusPetitEntier()
{
    dec string[] msg = "11.testPlusPetitEntier : ";
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

fn void executerTestsEntier()
{
    call print(call testEgaliteEntier()); call backSlashN();
    call print(call testInegaliteEntier()); call backSlashN();
    call print(call testPlusGrandEntier()); call backSlashN();
    call print(call testPlusPetitEntier()); call backSlashN();
    call print(call testValeurAbsolue()); call backSlashN();
    call print(call testMaximum()); call backSlashN();
    call print(call testMinimum()); call backSlashN();
}
