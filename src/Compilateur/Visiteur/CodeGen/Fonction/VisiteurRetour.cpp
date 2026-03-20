#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include <llvm-18/llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudRetour* noeudReturn)
{
    noeudReturn->getValeurRetour()->accept(this);
    llvm::Value* valeurEvaluee = _contextGenCode->getValeurTemporaire().getAdresse();
    IType* typeRetourObj = _contextGenCode->getReturnContextCompilation()->recupererContext();
    llvm::Type* typeRetourLLVM = typeRetourObj->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
    llvm::Value* valeurRetour = _contextGenCode->getBackend()->creerAutoCast(valeurEvaluee, typeRetourLLVM);
    _contextGenCode->getBackend()->getBuilder().CreateRet(valeurRetour);
    _contextGenCode->modifierValeurTemporaire(Symbole(valeurRetour, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}