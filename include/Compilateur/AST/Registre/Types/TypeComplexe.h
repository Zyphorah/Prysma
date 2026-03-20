#ifndef C747FE4B_9A53_40BA_A4BE_19C00508C8C4
#define C747FE4B_9A53_40BA_A4BE_19C00508C8C4

#include "IType.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <string>

class TypeComplexe : public IType {
private:
    std::string nomClasse;
public:
    explicit TypeComplexe(const std::string& nom);
    
    auto genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto estFlottant() const -> bool override;
    [[nodiscard]] auto estBooleen() const -> bool override;
    [[nodiscard]] auto estChaine() const -> bool override;
    [[nodiscard]] auto estTableau() const -> bool override;
    [[nodiscard]] auto estComplexe() const -> bool override;

    [[nodiscard]] static auto classof(const IType* type) -> bool { return type->estComplexe(); }
    
    auto getVTableType(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto getNomClasse() const -> std::string;
};

#endif /* C747FE4B_9A53_40BA_A4BE_19C00508C8C4 */
