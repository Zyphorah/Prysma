#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <stdexcept>

void VisiteurGeneralGenCode::visiter(NoeudUnRefVariable* noeudUnRefVariable) 
{
    Token tokenRecherche;
    tokenRecherche.value = noeudUnRefVariable->_nomVariable;

    llvm::Value* adresseMemoire = _contextGenCode->registreVariable->recupererVariables(tokenRecherche);

    if (adresseMemoire == nullptr) {
        throw std::runtime_error("Erreur sémantique : La variable '" + noeudUnRefVariable->_nomVariable + "' n'est pas déclarée.");
    }

    llvm::Type* typeDonnee = nullptr;

    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        typeDonnee = allocaInst->getAllocatedType();
    } else {
        typeDonnee = llvm::Type::getFloatTy(_contextGenCode->backend->getContext()); 
    }

    _contextGenCode->valeurTemporaire = _contextGenCode->backend->getBuilder().CreateLoad(
        typeDonnee, 
        adresseMemoire, 
        noeudUnRefVariable->_nomVariable 
    );
}