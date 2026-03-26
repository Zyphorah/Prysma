#ifndef D5B94044_819C_4C15_B528_AAE97CBCB264
#define D5B94044_819C_4C15_B528_AAE97CBCB264

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <string>

class VariableTypeExtractor {
private:
    ContextGenCode* _context;
public:
    explicit VariableTypeExtractor(ContextGenCode* context);
    auto extract(llvm::Value* memoryAddress) -> llvm::Type*;
};

class VariableAddressor {
private:
    ContextGenCode* _context;
public:
    explicit VariableAddressor(ContextGenCode* context);
    auto getAddress(const std::string& variableName) -> Symbol;
};

class VariableLoader {
private:
    ContextGenCode* _context;
    VariableAddressor _addressor;
    VariableTypeExtractor _extractor;
public:
    explicit VariableLoader(ContextGenCode* context);
    auto load(const std::string& variableName) -> Symbol;
    auto loadUnref(const std::string& variableName) -> Symbol;
};

class VariableAllocator {
private:
    ContextGenCode* _context;
public:
    explicit VariableAllocator(ContextGenCode* context);
    auto allocate(llvm::Type* type, const std::string& variableName) -> llvm::AllocaInst*;
    void store(llvm::Value* value, llvm::AllocaInst* allocaInst);
};

class VariableAssigner {
private:
    ContextGenCode* _context;
    VariableTypeExtractor _extractor;
public:
    explicit VariableAssigner(ContextGenCode* context);
    void assign(llvm::Value* existingVariable, llvm::Value* value, llvm::Type* llvmVariableType = nullptr);
};

#endif /* D5B94044_819C_4C15_B528_AAE97CBCB264 */
