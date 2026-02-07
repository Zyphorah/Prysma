#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Operande/Valeur.h"

void VisiteurGeneralGenCode::visiter(Valeur* valeur) 
{
    _contextGenCode->valeurTemporaire = valeur->_valeur;
}
