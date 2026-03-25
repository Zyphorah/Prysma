#include "Compiler/AST/Registry/Types/TypeComplex.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <string>

TypeComplex::TypeComplex(const std::string& nom) : nomClasse(nom) {}

auto TypeComplex::generatedrTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplex::estFlottant() const -> bool { return false; }
auto TypeComplex::estBooleen() const -> bool { return false; }
auto TypeComplex::estChaine() const -> bool { return false; }
auto TypeComplex::estArray() const -> bool { return false; }
auto TypeComplex::estComplex() const -> bool { return true; }

auto TypeComplex::getVTableType(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplex::getNomClasse() const -> std::string {
    return nomClasse;
}