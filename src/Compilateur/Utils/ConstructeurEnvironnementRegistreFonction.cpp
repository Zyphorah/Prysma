#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"

ConstructeurEnvironnementRegistreFonction::ConstructeurEnvironnementRegistreFonction(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{
}

ConstructeurEnvironnementRegistreFonction::~ConstructeurEnvironnementRegistreFonction()
{
}

void ConstructeurEnvironnementRegistreFonction::remplir()
{   
    // Construire un nouveau registre avec des valeurs initialisée 
    auto* registreFonction = new RegistreFonction();

    for(const auto& cle : _contextGenCode->registreFonction->obtenirCles())
    {
        llvm::Type* funcType = _contextGenCode->registreFonction->recuperer(cle).typeRetour->genererTypeLLVM(_contextGenCode->backend->getContext());
        
        llvm::Function* vraieFonction = llvm::Function::Create(
            (llvm::FunctionType*)funcType, 
            llvm::Function::ExternalLinkage, 
            cle,          
            _contextGenCode->backend->getModule()
        );

        SymboleFonction symboleFonction;

        // Construire la fonction non nullptr 
        symboleFonction.fonction = vraieFonction;
        symboleFonction.typeRetour = _contextGenCode->registreFonction->recuperer(cle).typeRetour;
        symboleFonction.noeud = _contextGenCode->registreFonction->recuperer(cle).noeud;
        registreFonction->enregistrer(cle, symboleFonction);        
    }
    _contextGenCode->registreFonction = registreFonction;
}