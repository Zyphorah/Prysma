#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/AST/AST_Genere.h"
#include <llvm/IR/DerivedTypes.h>
#include <stdexcept>
#include <utility>

TypeTableau::TypeTableau(IType* typeEnfant, INoeud* taille)
    : _typeEnfant(typeEnfant), _taille(taille)
{
}

llvm::Type* TypeTableau::genererTypeLLVM(llvm::LLVMContext& context)
{
    llvm::Type* typeElement = _typeEnfant->genererTypeLLVM(context);

    // Si la taille est nullptr (tableau à taille dynamique), on retourne nullptr
    // La taille réelle sera calculée lors de l'initialisation
    if (_taille == nullptr) {
        return nullptr;
    }

    NoeudLitteral* litteral = nullptr;

    if (_taille->getTypeGenere() == NoeudTypeGenere::Litteral) {
        litteral = static_cast<NoeudLitteral*>(_taille);
    }
    
    if (litteral == nullptr) {
        throw std::runtime_error("Erreur : la taille du tableau doit être un littéral entier");
    }

    uint64_t taille = static_cast<uint64_t>(std::stoi(litteral->getToken().value));
    return llvm::ArrayType::get(typeElement, taille);
}

bool TypeTableau::estFlottant() const
{
    return false;
}

bool TypeTableau::estBooleen() const
{
    return false;
}

bool TypeTableau::estChaine() const
{
    return _typeEnfant->estChaine();
}
