include "TestLoop.p";
include "TestCondition.p";
include "TestFunctionalVariableBool.p";
include "TestFunctionalVariableFloat.p";
include "TestFunctionalVariableInt.p";
include "TestRecursion.p";
include "TestArrayBool.p";
include "TestArrayFloat.p";
include "TestArrayInt.p";
include "TestMemoryDynamic.p";



fn int64 main() 
{
    call testLoop();
    call testCondition();
    call testFunctionalVariableBool();
    call testFunctionalVariableFloat();
    call testFunctionalVariableInt();
    call testRecursivite();
    call testArrayBool();
    call testArrayFloat();
    call testArrayInt();
    call testMemoryDynamic();
    
    return 0;
}