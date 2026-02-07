#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"


void VisiteurGeneralGenCode::visiter(NoeudDeclarationFonction* noeudDeclarationFonction) 
{
    /*
     if (_backend == nullptr) {
        throw std::runtime_error("Erreur : backend LLVM non initialisé dans NoeudDeclarationFonction");
    }

    llvm::Type* retType = _registreType->getType(_typeRetourToken);
    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, false);

    llvm::Function* function = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        _nom,
        &_backend->getModule()
    );

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_backend->getContext(), "entry", function);
    _backend->getBuilder().SetInsertPoint(entryBlock);

    _registreFonction->enregistrer(_nom, function);

    _returnContextCompilation->piler(_typeRetourToken);
    _registreVariable->piler();

    executerEnfants();

    _registreVariable->depiler();
    _returnContextCompilation->depiler();

    return function;
    */
}