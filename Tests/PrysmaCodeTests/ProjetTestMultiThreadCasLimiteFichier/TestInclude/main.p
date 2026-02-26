include "Math/Math.p";
include "Validation/Validation.p";

fn int32 main()
{
    dec string[] titre = "=== Tests Include ===";
    call print(ref titre); call backSlashN();

    dec string[] titreMath = "--- Math ---";
    call print(ref titreMath); call backSlashN();
    call executerTestsMath();

    dec string[] titreValidation = "--- Validation ---";
    call print(ref titreValidation); call backSlashN();
    call executerTestsValidation();

    dec string[] fin = "=== Fin ===";
    call print(ref fin); call backSlashN();
    return 0;
}
