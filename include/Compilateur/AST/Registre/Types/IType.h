#ifndef BFF76A14_E9C5_4CB2_820B_010F0D0924A1
#define BFF76A14_E9C5_4CB2_820B_010F0D0924A1

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>


class IType {
public:
    IType() = default;
    IType(const IType&) = delete;
    auto operator=(const IType&) -> IType& = delete;
    IType(IType&&) = delete;
    auto operator=(IType&&) -> IType& = delete;
    virtual ~IType() = default;

    virtual auto genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* = 0;
    
    // Méthodes utilitaires pour déterminer les caractéristiques du type
    [[nodiscard]] virtual auto estFlottant() const -> bool = 0;
    [[nodiscard]] virtual auto estBooleen() const -> bool = 0;
    [[nodiscard]] virtual auto estChaine() const -> bool = 0;
    [[nodiscard]] virtual auto estTableau() const -> bool = 0;
    [[nodiscard]] virtual auto estComplexe() const -> bool { return false; }
    virtual auto getVTableType(::llvm::LLVMContext& /*context*/) -> ::llvm::Type* { return nullptr; }
};

#endif /* BFF76A14_E9C5_4CB2_820B_010F0D0924A1 */
