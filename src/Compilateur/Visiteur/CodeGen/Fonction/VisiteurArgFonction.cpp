#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"

void VisiteurGeneralGenCode::visiter([[maybe_unused]]NoeudArgFonction* noeudArgFonction) 
{
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}