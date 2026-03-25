fn bool testEgaliteFloat()
{
    dec string[] msg = "15.testEgaliteFloat : ";
    call print(ref msg);
    dec bool r = call estEgalFloat(3.14, 3.14);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testInegaliteFloat()
{
    dec string[] msg = "16.testInegaliteFloat : ";
    call print(ref msg);
    dec bool r = call estEgalFloat(1.0, 2.0);
    if (r == false) {
        return true;
    }
    return false;
}

fn bool testPlusGrandFloat()
{
    dec string[] msg = "17.testPlusGrandFloat : ";
    call print(ref msg);
    dec bool r = call estPlusGrandFloat(9.5, 2.3);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testPlusPetitFloat()
{
    dec string[] msg = "18.testPlusPetitFloat : ";
    call print(ref msg);
    dec bool r = call estPlusPetitFloat(0.5, 1.0);
    if (r == true) {
        return true;
    }
    return false;
}

fn bool testComparisonChaineFloat()
{
    dec string[] msg = "19.testComparisonChaineFloat : ";
    call print(ref msg);
    dec float a = 5.0;
    dec float b = 10.0;
    dec float c = 5.0;
    dec bool ab = call estPlusPetitFloat(a, b);
    dec bool bc = call estPlusGrandFloat(b, c);
    dec bool ac = call estEgalFloat(a, c);
    if (ab == true) {
        if (bc == true) {
            if (ac == true) {
                return true;
            }
        }
    }
    return false;
}

fn void executerTestsFloat()
{
    call print(call testEgaliteFloat()); call backSlashN();
    call print(call testInegaliteFloat()); call backSlashN();
    call print(call testPlusGrandFloat()); call backSlashN();
    call print(call testPlusPetitFloat()); call backSlashN();
    call print(call testComparisonChaineFloat()); call backSlashN();
}
