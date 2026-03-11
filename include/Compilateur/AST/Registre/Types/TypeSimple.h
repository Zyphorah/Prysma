#ifndef D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB
#define D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB

#include "IType.h"

class TypeSimple : public IType {
private:
    llvm::Type* _typeLLVM;

public:
    explicit TypeSimple(llvm::Type* typeLLVM);
    ~TypeSimple() override = default;

    llvm::Type* genererTypeLLVM(llvm::LLVMContext& context) override;
    
    bool estFlottant() const override;
    bool estBooleen() const override;
    bool estChaine() const override;
    bool estTableau() const override { return false; }
};

#endif /* D3CF3339_1CBC_4EEE_9EE7_B2E99140A4CB */
