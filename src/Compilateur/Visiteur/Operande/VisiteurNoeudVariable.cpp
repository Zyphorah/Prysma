#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"


void VisiteurGeneralGenCode::visiter(NoeudVariable* noeudVariable)
{
    /*
    if (_registre == nullptr) {
        throw std::runtime_error("ERREUR: NoeudVariable::genCode() - Le registre de variables est NULL ! ""La variable '" + _nomVariable );
    }

    Token tokenRecherche;
    tokenRecherche.value = _nomVariable;

    llvm::Value* adresseMemoire = _registre->recupererVariables(tokenRecherche);

    if (adresseMemoire == nullptr) {
        throw std::runtime_error("Erreur sémantique : La variable '" + _nomVariable + "' n'est pas déclarée.");
    }

    llvm::Type* typeDonnee = nullptr;
    

    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        typeDonnee = allocaInst->getAllocatedType();
    } else {
        
        typeDonnee = llvm::Type::getFloatTy(_backend->getContext()); 
    }
 
    return _backend->getBuilder().CreateLoad(
        typeDonnee, 
        adresseMemoire, 
        _nomVariable 
    );
    */
}