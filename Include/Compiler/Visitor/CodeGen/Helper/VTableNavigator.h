#ifndef VTABLE_NAVIGATOR_H
#define VTABLE_NAVIGATOR_H

#include <llvm-18/llvm/IR/Value.h>
#include <llvm-18/llvm/IR/IRBuilder.h>

class VTableNavigator {
private:
    llvm::IRBuilderBase* builder;

public:
    explicit VTableNavigator(llvm::IRBuilderBase* p_builder);
    auto getMethodPointer(llvm::Value* vtable, llvm::Type* vtableType, int methodIndex) -> llvm::Value*;
};

#endif
