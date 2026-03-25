#include "Compiler/Visitor/CodeGen/Helper/VTableNavigator.h"
#include <cstdint>
#include <llvm-18/llvm/IR/IRBuilder.h>
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

VTableNavigator::VTableNavigator(llvm::IRBuilderBase* p_builder) : builder(p_builder) {}

auto VTableNavigator::recupererPointerMethode(llvm::Value* vtable, llvm::Type* vtableType, int indexMethode) -> llvm::Value* {
    llvm::Value* indexValue = builder->getInt32(static_cast<uint32_t>(indexMethode));
    std::vector<llvm::Value*> indices;
    indices.push_back(builder->getInt32(0));
    indices.push_back(indexValue);
    
    llvm::Value* pointeurGep = builder->CreateInBoundsGEP(vtableType, vtable, indices, "vtable_gep");
    return builder->CreateLoad(builder->getPtrTy(), pointeurGep, "methode_ptr");
}
