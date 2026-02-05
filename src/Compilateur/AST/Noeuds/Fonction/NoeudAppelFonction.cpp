#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/LLVM/LLVMBackend.h"


NoeudAppelFonction::NoeudAppelFonction(Token nomFonction, std::shared_ptr<RegistreFonction> registreFonction,std::shared_ptr<LLVMBackend> backend)
{
    _nomFonction = std::move(nomFonction);
    _registreFonction = std::move(registreFonction);
    _backend = std::move(backend);
}

NoeudAppelFonction::~NoeudAppelFonction()
{}

llvm::Value* NoeudAppelFonction::genCode()
{
    llvm::Function* fonction = _registreFonction->obtenir(_nomFonction.value);
    return _backend->getBuilder().CreateCall(fonction, {}, "resultat_appel");
}