#include "Compilateur/LLVM/GestionVariable.h"
#include <stdexcept>

GestionVariable::GestionVariable(std::shared_ptr<ContextGenCode> contextGenCode)
    : _contextGenCode(std::move(contextGenCode))
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

llvm::Value* GestionVariable::chargerVariable(const std::string& nomVariable)
{
    Token tokenRecherche;
    tokenRecherche.value = nomVariable;

    llvm::Value* adresseMemoire = _contextGenCode->registreVariable->recupererVariables(tokenRecherche);

    if (adresseMemoire == nullptr) {
        throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
    }

    llvm::Type* typeDonnee = extraireTypeDonnee(adresseMemoire);

    llvm::Value* valeurChargee = _contextGenCode->backend->getBuilder().CreateLoad(
        typeDonnee,
        adresseMemoire,
        nomVariable
    );

    return valeurChargee;
}

llvm::AllocaInst* GestionVariable::allouerVariable(llvm::Type* type, const std::string& nomVariable)
{
    llvm::BasicBlock* insertBlock = _contextGenCode->backend->getBuilder().GetInsertBlock();
    _contextGenCode->backend->definirPointInsertionApresAllocation();

    llvm::AllocaInst* allocaInst = _contextGenCode->backend->getBuilder().CreateAlloca(type, nullptr, nomVariable);
    
    _contextGenCode->backend->getBuilder().SetInsertPoint(insertBlock);

    return allocaInst;
}

void GestionVariable::enregistrerVariable(const std::string& nomVariable, llvm::AllocaInst* allocaInst)
{
    Token nomDeLaVariable;
    nomDeLaVariable.value = nomVariable;
    nomDeLaVariable.type = TOKEN_IDENTIFIANT;

    _contextGenCode->registreVariable->enregistrer(nomDeLaVariable, allocaInst);
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
