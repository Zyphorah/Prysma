#include "Compiler/AST/Registry/Types/TypeComplex.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <string>

TypeComplex::TypeComplex(const std::string& className) : className(className) {}

auto TypeComplex::generateLLVMType(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplex::isFloating() const -> bool { return false; }
auto TypeComplex::isBoolean() const -> bool { return false; }
auto TypeComplex::isString() const -> bool { return false; }
auto TypeComplex::isArray() const -> bool { return false; }
auto TypeComplex::isComplex() const -> bool { return true; }

auto TypeComplex::getVTableType(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplex::getClassName() const -> std::string {
    return className;
}