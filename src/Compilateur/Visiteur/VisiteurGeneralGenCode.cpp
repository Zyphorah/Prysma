#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include <llvm/IR/Constants.h>

VisiteurGeneralGenCode::VisiteurGeneralGenCode(std::shared_ptr<ContextGenCode> contextGenCode) : _contextGenCode(std::move(contextGenCode))
{}

VisiteurGeneralGenCode::~VisiteurGeneralGenCode()
{}


void VisiteurGeneralGenCode::parcourirEnfant(NoeudInstruction* noeud)
{
       for (const auto& enfant : noeud->getEnfants()) {
        enfant->accept(this);
    }
}

llvm::Value* VisiteurGeneralGenCode::evaluerExpression(std::shared_ptr<INoeud>& expression) {
    if (expression != nullptr) {
        expression->accept(this);
        return _contextGenCode->valeurTemporaire;
    }
    return nullptr;
}

void VisiteurGeneralGenCode::visiter(NoeudLitteral* noeudLitteral)
{
    float valeur = noeudLitteral->getValeur();
    llvm::Value* valeurLLVM = llvm::ConstantFP::get(
        llvm::Type::getFloatTy(_contextGenCode->backend->getContext()), 
        valeur
    );
    _contextGenCode->valeurTemporaire = valeurLLVM;
}

