include "Outils/Outils.p";
include "ValiderEntier.p";
include "ValiderFloat.p";

fn void executerTestsValidation()
{
    call executerTestsEntier();
    call executerTestsFloat();
}
