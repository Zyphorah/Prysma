#include "Compilateur/LLVM/GestionVariable.h"
#include <stdexcept>

GestionVariable::GestionVariable(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{
}

GestionVariable::~GestionVariable()
{
}

llvm::Type* GestionVariable::extraireTypeDonnee(llvm::Value* adresseMemoire)
{
    llvm::Type* typeDonnee = nullptr;

    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        typeDonnee = allocaInst->getAllocatedType();
    } else {
        typeDonnee = llvm::Type::getFloatTy(_contextGenCode->backend->getContext());
    }

    return typeDonnee;
}

Symbole GestionVariable::chargerVariable(const std::string& nomVariable)
{
    Token tokenRecherche;
    tokenRecherche.value = nomVariable;

    Symbole symbole = _contextGenCode->registreVariable->recupererVariables(tokenRecherche);
    llvm::Value* adresseMemoire = symbole.adresse;

    if (adresseMemoire == nullptr) {
        throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
    }

    llvm::Type* typeDonnee = extraireTypeDonnee(adresseMemoire);

    llvm::Value* valeurChargee = _contextGenCode->backend->getBuilder().CreateLoad(
        typeDonnee,
        adresseMemoire,
        nomVariable
    );

    symbole.adresse = valeurChargee;
    return symbole;
}

llvm::AllocaInst* GestionVariable::allouerVariable(llvm::Type* type, const std::string& nomVariable)
{
    llvm::BasicBlock* insertBlock = _contextGenCode->backend->getBuilder().GetInsertBlock();
    _contextGenCode->backend->definirPointInsertionApresAllocation();

    llvm::AllocaInst* allocaInst = _contextGenCode->backend->getBuilder().CreateAlloca(type, nullptr, nomVariable);
    
    _contextGenCode->backend->getBuilder().SetInsertPoint(insertBlock);

    return allocaInst;
}

void GestionVariable::stockerVariable(llvm::Value* valeur, llvm::AllocaInst* allocaInst)
{
    _contextGenCode->backend->getBuilder().CreateStore(valeur, allocaInst);
}

void GestionVariable::affecterVariable(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    llvm::Value* valeurCast = _contextGenCode->backend->creerAutoCast(valeur, allocaInst->getAllocatedType());
    _contextGenCode->backend->getBuilder().CreateStore(valeurCast, allocaInst);
}
