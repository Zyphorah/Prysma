fn bool testLoopConditionExtern()
{
    dec string[] functionnalite = "1.testLoopConditionExtern : ";
    call print(ref functionnalite);
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

fn bool testLoopNesPasEgale()
{
    dec string[] functionnalite = "2.testLoopNesPasEgale : ";
    call print(ref functionnalite);
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

fn bool testLoopSimple()
{
    dec string[] functionnalite = "3.testLoop : ";
    call print(ref functionnalite);
    dec int32 i = 0;

    while (i < 5) {
        aff i = i + 1;
    }
    if (i == 5) {
        return true;
    } 
    return false;
}


fn bool testLoopImbrique()
{
    dec string[] functionnalite = "4.testLoopImbrique : ";
    call print(ref functionnalite);
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

fn bool testLoopConditionComplex()
{
    dec string[] functionnalite = "5.testLoopConditionComplex : ";
    call print(ref functionnalite);
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

fn bool testLoopNonExecute()
{
    dec string[] functionnalite = "6.testLoopNonExecute : ";
    call print(ref functionnalite);
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

fn int32 testLoop()
{
    call print(call testLoopSimple()); call backSlashN();
    call print(call testLoopNesPasEgale()); call backSlashN();
    call print(call testLoopConditionExtern()); call backSlashN();
    call print(call testLoopImbrique()); call backSlashN();
    call print(call testLoopConditionComplex()); call backSlashN();
    call print(call testLoopNonExecute()); call backSlashN();

    return 1;
}
