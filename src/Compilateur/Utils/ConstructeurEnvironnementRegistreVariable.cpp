#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "llvm/IR/GlobalVariable.h"

ConstructeurEnvironnementRegistreVariable::ConstructeurEnvironnementRegistreVariable(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{
}

ConstructeurEnvironnementRegistreVariable::~ConstructeurEnvironnementRegistreVariable()
{
}

void ConstructeurEnvironnementRegistreVariable::remplir()
{   
    auto* nouveauRegistre = new RegistreVariable();
    nouveauRegistre->piler();

    for(auto const& [nom, symbole] : _contextGenCode->registreVariable->getGlobalVariables())
    {
        llvm::Type* typeLLVM = symbole.type->genererTypeLLVM(_contextGenCode->backend->getContext());
        
        auto* variableGlobale = new llvm::GlobalVariable(
            _contextGenCode->backend->getModule(),
            typeLLVM,
            false,
            llvm::GlobalValue::ExternalLinkage,
            nullptr,
            nom
        );

        Token token;
        token.value = nom;
        nouveauRegistre->enregistrer(token, Symbole(variableGlobale, symbole.type));
    }
    
    delete _contextGenCode->registreVariable;
    _contextGenCode->registreVariable = nouveauRegistre;
}