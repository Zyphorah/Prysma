#ifndef D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB
#define D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB

#include "IType.h"
#include <llvm/IR/Type.h>

class TypeSimple : public IType {
private:
    llvm::Type* _typeLLVM;

public:
    explicit TypeSimple(llvm::Type* typeLLVM);
    ~TypeSimple() override = default;

    TypeSimple(const TypeSimple&) = delete;
    auto operator=(const TypeSimple&) -> TypeSimple& = delete;
    TypeSimple(TypeSimple&&) = delete;
    auto operator=(TypeSimple&&) -> TypeSimple& = delete;

    auto genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto estFlottant() const -> bool override;
    [[nodiscard]] auto estBooleen() const -> bool override;
    [[nodiscard]] auto estChaine() const -> bool override;
    [[nodiscard]] auto estTableau() const -> bool override { return false; }
};

#endif /* D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB */
