#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include <llvm/IR/DerivedTypes.h>
#include <stdexcept>
#include <utility>

TypeTableau::TypeTableau(std::shared_ptr<IType> typeEnfant, std::shared_ptr<INoeud> taille)
    : _typeEnfant(std::move(typeEnfant)), _taille(std::move(taille))
{
}

llvm::Type* TypeTableau::genererTypeLLVM(llvm::LLVMContext& context)
{
    llvm::Type* typeElement = _typeEnfant->genererTypeLLVM(context);

    auto* litteral = dynamic_cast<NoeudLitteral*>(_taille.get());
    if (litteral == nullptr) {
        throw std::runtime_error("Erreur : la taille du tableau doit être un littéral entier");
    }

    uint64_t taille = static_cast<uint64_t>(std::stoi(litteral->getToken().value));
    return llvm::ArrayType::get(typeElement, taille);
}
