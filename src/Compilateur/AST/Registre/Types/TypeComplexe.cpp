#include "Compilateur/AST/Registre/Types/TypeComplexe.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <string>

TypeComplexe::TypeComplexe(const std::string& nom) : nomClasse(nom) {}

auto TypeComplexe::genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplexe::estFlottant() const -> bool { return false; }
auto TypeComplexe::estBooleen() const -> bool { return false; }
auto TypeComplexe::estChaine() const -> bool { return false; }
auto TypeComplexe::estTableau() const -> bool { return false; }
auto TypeComplexe::estComplexe() const -> bool { return true; }

auto TypeComplexe::getVTableType(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplexe::getNomClasse() const -> std::string {
    return nomClasse;
}