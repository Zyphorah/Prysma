#include "Compiler/AST/Utils/BuilderEnvironmentRegistryVariable.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/LLVM/LlvmBackend.h"
#include "Compiler/Lexer/Lexer.h"
#include "llvm/IR/GlobalVariable.h"
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Type.h>

BuilderEnvironmentRegistryVariable::BuilderEnvironmentRegistryVariable(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

BuilderEnvironmentRegistryVariable::~BuilderEnvironmentRegistryVariable() {}

// Actuellement inutilisée, c'est pour le future, un autre ittération de développement, elle existe pour le moment mais elle n'est pas encore testé
// Une functionnalité bonus 
// Cette classe a pour but de remplir le registry de variable global ce trouvant à l'extérieur d'une function 
// exemple : 
// scope {
//     dec int64 a = 10;
//     dec string b = "string";
//     fn void teste()
//     {
//     }
// }

void BuilderEnvironmentRegistryVariable::remplir()
{   
    auto variablesGlobalesPasse1 = _contextGenCode->getRegistryVariable()->getGlobalVariables();

    _contextGenCode->getRegistryVariable()->viderTop(); 

    for(auto const& [nom, symbole] : variablesGlobalesPasse1)
    {
        llvm::Type* typeLLVM = symbole.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        
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
        
        _contextGenCode->getRegistryVariable()->enregistryr(token, Symbole(variableGlobale, symbole.getType()));
    }
}