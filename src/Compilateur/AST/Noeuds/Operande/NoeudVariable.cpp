#include "Compilateur/AST/Noeuds/Operande/NoeudVariable.h"
#include <stdexcept>
#include <utility>
#include <llvm/IR/Instructions.h> 


NoeudVariable::NoeudVariable(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registre, const std::string& nomVariable)
    : _backend(std::move(backend)), _registre(std::move(registre)), _nomVariable(nomVariable)
{
}

NoeudVariable::~NoeudVariable()
{
}

void NoeudVariable::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

llvm::Value* NoeudVariable::genCode()
{
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
}