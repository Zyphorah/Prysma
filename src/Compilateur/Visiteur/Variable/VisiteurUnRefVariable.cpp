#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudUnRefVariable* noeudUnRefVariable) 
{
    GestionVariable gestionVariable(_contextGenCode);
    
    _contextGenCode->valeurTemporaire = gestionVariable.chargerVariable(noeudUnRefVariable->_nomVariable);
}