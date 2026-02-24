
fn bool testBoucleConditionExtern()
{
    dec string[] fonctionnalite = "1.testBoucleConditionExtern : ";
    call print(ref fonctionnalite);
    dec int32 i = 0; 

    dec bool condition = i < 5;

    while(condition)
    {
        aff i = i + 1; 
        aff condition = i < 5;
    }
    if (i == 5)
    {
        return true;    
    }
    return false;
}

fn bool testBoucleNesPasEgale()
{
    dec string[] fonctionnalite = "2.testBoucleNesPasEgale : ";
    call print(ref fonctionnalite);
    dec int32 i = 0; 
    while(i != 5)
    {
        aff i = i +1; 
    }
    if (i == 5)
    {
        return true;
    }
    return false; 
}

fn bool testBoucle()
{
    dec string[] fonctionnalite = "3.testBoucle : ";
    call print(ref fonctionnalite);
    dec int32 i = 0;

    while (i < 5) {
        aff i = i + 1;
    }
    if (i == 5) {
        return true;
    } 
    return false;
}


fn bool testBoucleImbrique()
{
    dec string[] fonctionnalite = "4.testBoucleImbrique : ";
    call print(ref fonctionnalite);
    dec int32 i = 0;
    dec int32 j = 0;
    dec int32 total = 0;
    while(i < 5)
    {
        aff j = 0;
        while(j < 5)
        {
            aff total = total + 1;
            aff j = j + 1;
        }
        aff i = i + 1;
    }
    if(total == 25)
    {
        return true;
    }
    return false;
}

fn bool testBoucleConditionComplexe()
{
    dec string[] fonctionnalite = "5.testBoucleConditionComplexe : ";
    call print(ref fonctionnalite);
    dec int32 i = 0;
    dec int32 j = 10;
    while(i < 5 && j > 5)
    {
        aff i = i + 1;
        aff j = j - 1;
    }
    if(i == 5 && j == 5)
    {
        return true;
    }
    return false;
}

fn bool testBoucleNonExecute()
{
    dec string[] fonctionnalite = "6.testBoucleNonExecute : ";
    call print(ref fonctionnalite);
    dec int32 i = 5;
    while(i < 5)
    {
        aff i = i + 1;
    }
    if(i == 5)
    {
        return true;
    }
    return false;
}

fn int32 main()
{
    call print(call testBoucle()); call backSlashN();
    call print(call testBoucleNesPasEgale()); call backSlashN();
    call print(call testBoucleConditionExtern()); call backSlashN();
    call print(call testBoucleImbrique()); call backSlashN();
    call print(call testBoucleConditionComplexe()); call backSlashN();
    call print(call testBoucleNonExecute()); call backSlashN();

    return 1;
}
