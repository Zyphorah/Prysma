include "Math/Math.p";
include "Validation/Validation.p";

fn int32 main()
{
    dec string[] title = "=== Tests Include ===";
    call print(ref title); call backSlashN();

    dec string[] titleMath = "--- Math ---";
    call print(ref titleMath); call backSlashN();
    call runMathTests();

    dec string[] titleValidation = "--- Validation ---";
    call print(ref titleValidation); call backSlashN();
    call runValidationTests();

    dec string[] msgEnd = "=== End ===";
    call print(ref msgEnd); call backSlashN();
    return 0;
}
