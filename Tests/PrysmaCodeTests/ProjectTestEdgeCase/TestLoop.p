fn bool testLoopConditionExternal()
{
    dec string[] feature = "1.testLoopConditionExternal : ";
    call print(ref feature);
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

fn bool testLoopNotEqual()
{
    dec string[] feature = "2.testLoopNotEqual : ";
    call print(ref feature);
    dec int32 i = 0; 
    while(i != 5)
    {
        aff i = i + 1; 
    }
    if (i == 5)
    {
        return true;
    }
    return false; 
}

fn bool testLoopSimple()
{
    dec string[] feature = "3.testLoop : ";
    call print(ref feature);
    dec int32 i = 0;

    while (i < 5) {
        aff i = i + 1;
    }
    if (i == 5) {
        return true;
    } 
    return false;
}


fn bool testLoopNested()
{
    dec string[] feature = "4.testLoopNested : ";
    call print(ref feature);
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
    dec string[] feature = "5.testLoopConditionComplex : ";
    call print(ref feature);
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

fn bool testLoopNotExecuted()
{
    dec string[] feature = "6.testLoopNotExecuted : ";
    call print(ref feature);
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
    call print(call testLoopNotEqual()); call backSlashN();
    call print(call testLoopConditionExternal()); call backSlashN();
    call print(call testLoopNested()); call backSlashN();
    call print(call testLoopConditionComplex()); call backSlashN();
    call print(call testLoopNotExecuted()); call backSlashN();

    return 1;
}
