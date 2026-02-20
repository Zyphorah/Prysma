#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudRetour.h"
#include "Compilateur/AST/Registre/Types/IType.h"

void VisiteurGeneralGenCode::visiter(NoeudRetour* noeudReturn)
{
    noeudReturn->getValeurRetour()->accept(this);
    llvm::Value* valeurEvaluee = _contextGenCode->valeurTemporaire;
    IType* typeRetourObj = _contextGenCode->returnContextCompilation->recupererContext();
    llvm::Type* typeRetourLLVM = typeRetourObj->genererTypeLLVM(_contextGenCode->backend->getContext());
    llvm::Value* valeurRetour = _contextGenCode->backend->creerAutoCast(valeurEvaluee, typeRetourLLVM);
    _contextGenCode->backend->getBuilder().CreateRet(valeurRetour);
    _contextGenCode->valeurTemporaire = valeurRetour;
}