include "TestAllocationDynamicMemory/TestNewDeleteInt32.p";
include "TestAllocationDynamicMemory/TestNewDeleteInt64.p";
include "TestAllocationDynamicMemory/TestNewDeleteFloat.p";
include "TestAllocationDynamicMemory/TestNewDeleteBool.p";
include "TestAllocationDynamicMemory/TestPassagePtrArgument.p";

fn int32 testMemoryDynamic()
{
   call testNewDeleteInt32();
   call testNewDeleteInt64();
   call testNewDeleteFloat();
   call testNewDeleteBool();
   call testPassPtrArgument();

   return 1;
}
