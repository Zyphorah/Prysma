#ifndef C747FE4B_9A53_40BA_A4BE_19C00508C8C4
#define C747FE4B_9A53_40BA_A4BE_19C00508C8C4

#include "IType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <string>

class TypeComplex : public IType {
private:
    std::string nomClasse;
public:
    explicit TypeComplex(const std::string& nom);
    
    auto generatedrTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto estFlottant() const -> bool override;
    [[nodiscard]] auto estBooleen() const -> bool override;
    [[nodiscard]] auto estChaine() const -> bool override;
    [[nodiscard]] auto estArray() const -> bool override;
    [[nodiscard]] auto estComplex() const -> bool override;

    [[nodiscard]] static auto classof(const IType* type) -> bool { return type->estComplex(); }
    
    auto getVTableType(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto getNomClasse() const -> std::string;
};

#endif /* C747FE4B_9A53_40BA_A4BE_19C00508C8C4 */
