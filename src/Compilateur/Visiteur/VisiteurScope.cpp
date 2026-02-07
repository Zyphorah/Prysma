#include "Compilateur/AST/Noeuds/NoeudScope.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

class NoeudScope;

void VisiteurGeneralGenCode::visiter(NoeudScope* noeudScope)
{
    parcourirEnfant(noeudScope);
}