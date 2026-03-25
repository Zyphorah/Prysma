#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeDelete* nodeDelete)
{
    auto& module = _contextGenCode->getBackend()->getModule();
    auto& builder = _contextGenCode->getBackend()->getBuilder();

    // Récupérer le token de la variable à supprimer
    const Token& tokenVariable = nodeDelete->getNomType();

    // Chercher la variable dans le registry de variables pour déterminer si elle existe
    Symbole symbole = _contextGenCode->getRegistryVariable()->recupererVariables(tokenVariable);

    ErrorHelper::verifierNonNull(symbole.getAdresse(), "Variable '" + tokenVariable.value + "' non déclarée");
    
    llvm::Value* adresseMemory = symbole.getAdresse();
    llvm::Type* typeDonnee = adresseMemory->getType();

    if (!typeDonnee->isPointerTy()) {
        ErrorHelper::errorCompilation("Variable '" + tokenVariable.value + "' n'est pas un pointeur (delete requiert un pointeur)");
    }

    llvm::Value* adresseALiberer = builder.CreateLoad(
        typeDonnee,
        adresseMemory,
        "pointeur_" + tokenVariable.value
    );

    // Récupérer la function prysma_free du module
    llvm::Function* freeFunc = module.getFunction("prysma_free");

    freeFunc = ErrorHelper::verifierNonNull(freeFunc, "Function prysma_free non déclarée");

    // Caller prysma_free avec l'adresse à libérer
    builder.CreateCall(freeFunc, {adresseALiberer});

    // Réinitialiser la valeur temporaire
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}
