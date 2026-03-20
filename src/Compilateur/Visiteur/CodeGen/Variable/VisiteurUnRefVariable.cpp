#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudUnRefVariable* noeudUnRefVariable) 
{
    ChargeurVariable chargeur(_contextGenCode);
    
    _contextGenCode->modifierValeurTemporaire(chargeur.chargerUnref(noeudUnRefVariable->getNomVariable()));
}