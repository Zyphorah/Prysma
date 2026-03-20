#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include <cstdint>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <stdexcept>
#include <string>

TypeTableau::TypeTableau(IType* typeEnfant, INoeud* taille)
    : _typeEnfant(typeEnfant), _taille(taille)
{
}

auto TypeTableau::genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type*
{
    llvm::Type* typeElement = _typeEnfant->genererTypeLLVM(context);

    // Si la taille est nullptr (tableau à taille dynamique), on retourne un pointeur
    // car en paramètre ou allocation dynamique, ça représente une adresse.
    if (_taille == nullptr) {
        return llvm::PointerType::getUnqual(context);
    }

    NoeudLitteral* litteral = nullptr;

    if (_taille->getTypeGenere() == NoeudTypeGenere::Litteral) {
        litteral = static_cast<NoeudLitteral*>(_taille); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    }
    
    if (litteral == nullptr) {
        throw std::runtime_error("Erreur : la taille du tableau doit être un littéral entier");
    }

    auto taille = static_cast<uint64_t>(std::stoi(litteral->getToken().value));
    return llvm::ArrayType::get(typeElement, taille);
}

auto TypeTableau::estFlottant() const -> bool
{
    return false;
}

auto TypeTableau::estBooleen() const -> bool
{
    return false;
}

auto TypeTableau::estChaine() const -> bool
{
    return _typeEnfant->estChaine();
}
