include "Tools/Tools.p";
include "ValidateInteger.p";
include "ValidateFloat.p";

fn void runValidationTests()
{
    call runIntegerTests();
    call runFloatTests();
}
