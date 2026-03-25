include "Addition.p";
include "Subtraction.p";
include "Multiplication.p";

fn bool testAddition()
{
    dec string[] msg = "1.testAddition : ";
    call print(ref msg);
    dec int32 r = call additionner(10, 20);
    if (r == 30) {
        return true;
    }
    return false;
}

fn bool testSoustraction()
{
    dec string[] msg = "2.testSoustraction : ";
    call print(ref msg);
    dec int32 r = call soustraire(50, 25);
    if (r == 25) {
        return true;
    }
    return false;
}

fn bool testMultiplication()
{
    dec string[] msg = "3.testMultiplication : ";
    call print(ref msg);
    dec int32 r = call multiplier(6, 7);
    if (r == 42) {
        return true;
    }
    return false;
}

fn bool testAdditionFloat()
{
    dec string[] msg = "4.testAdditionFloat : ";
    call print(ref msg);
    dec float r = call additionnerFloat(1.5, 2.5);
    if (r == 4.0) {
        return true;
    }
    return false;
}

fn bool testSoustractionFloat()
{
    dec string[] msg = "5.testSoustractionFloat : ";
    call print(ref msg);
    dec float r = call soustraireFloat(10.0, 3.5);
    if (r == 6.5) {
        return true;
    }
    return false;
}

fn bool testMultiplicationFloat()
{
    dec string[] msg = "6.testMultiplicationFloat : ";
    call print(ref msg);
    dec float r = call multiplierFloat(2.5, 4.0);
    if (r == 10.0) {
        return true;
    }
    return false;
}

fn bool testExpressionCombinee()
{
    dec string[] msg = "7.testExpressionCombinee : ";
    call print(ref msg);
    dec int32 a = call additionner(5, 3);
    dec int32 b = call multiplier(a, 2);
    dec int32 c = call soustraire(b, 6);
    if (c == 10) {
        return true;
    }
    return false;
}

fn void executerTestsMath()
{
    call print(call testAddition()); call backSlashN();
    call print(call testSoustraction()); call backSlashN();
    call print(call testMultiplication()); call backSlashN();
    call print(call testAdditionFloat()); call backSlashN();
    call print(call testSoustractionFloat()); call backSlashN();
    call print(call testMultiplicationFloat()); call backSlashN();
    call print(call testExpressionCombinee()); call backSlashN();
}
