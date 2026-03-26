#ifndef C747FE4B_9A53_40BA_A4BE_19C00508C8C4
#define C747FE4B_9A53_40BA_A4BE_19C00508C8C4

#include "IType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <string>

class TypeComplex : public IType {
private:
    std::string className;
public:
    explicit TypeComplex(const std::string& name);
    
    auto generateLLVMType(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto isFloating() const -> bool override;
    [[nodiscard]] auto isBoolean() const -> bool override;
    [[nodiscard]] auto isString() const -> bool override;
    [[nodiscard]] auto isArray() const -> bool override;
    [[nodiscard]] auto isComplex() const -> bool override;

    [[nodiscard]] static auto classof(const IType* type) -> bool { return type->isComplex(); }
    
    auto getVTableType(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto getClassName() const -> std::string;
};

#endif /* C747FE4B_9A53_40BA_A4BE_19C00508C8C4 */
