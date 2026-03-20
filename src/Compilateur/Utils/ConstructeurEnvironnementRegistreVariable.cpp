#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreVariable.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include "llvm/IR/GlobalVariable.h"
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Type.h>

ConstructeurEnvironnementRegistreVariable::ConstructeurEnvironnementRegistreVariable(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

ConstructeurEnvironnementRegistreVariable::~ConstructeurEnvironnementRegistreVariable() {}

// Actuellement inutilisée, c'est pour le future, un autre ittération de développement, elle existe pour le moment mais elle n'est pas encore testé
// Une fonctionnalité bonus 
// Cette classe a pour but de remplir le registre de variable global ce trouvant à l'extérieur d'une fonction 
// exemple : 
// scope {
//     dec int64 a = 10;
//     dec string b = "string";
//     fn void teste()
//     {
//     }
// }

void ConstructeurEnvironnementRegistreVariable::remplir()
{   
    auto variablesGlobalesPasse1 = _contextGenCode->getRegistreVariable()->getGlobalVariables();

    _contextGenCode->getRegistreVariable()->viderTop(); 

    for(auto const& [nom, symbole] : variablesGlobalesPasse1)
    {
        llvm::Type* typeLLVM = symbole.getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
        
        auto* variableGlobale = new llvm::GlobalVariable(
            _contextGenCode->getBackend()->getModule(),
            typeLLVM,
            false,
            llvm::GlobalValue::ExternalLinkage,
            nullptr,
            nom
        );

        Token token;
        token.value = nom;
        
        _contextGenCode->getRegistreVariable()->enregistrer(token, Symbole(variableGlobale, symbole.getType()));
    }
}