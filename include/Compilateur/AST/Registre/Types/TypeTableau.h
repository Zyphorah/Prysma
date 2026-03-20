#ifndef B05A551C_2519_458F_BB1D_8E5EF8DB9B83
#define B05A551C_2519_458F_BB1D_8E5EF8DB9B83

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "IType.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

class TypeTableau : public IType {
private:
    IType* _typeEnfant;
    INoeud* _taille; 

public:
    TypeTableau(IType* typeEnfant, INoeud* taille);
    ~TypeTableau() override = default;

    TypeTableau(const TypeTableau&) = delete;
    auto operator=(const TypeTableau&) -> TypeTableau& = delete;
    TypeTableau(TypeTableau&&) = delete;
    auto operator=(TypeTableau&&) -> TypeTableau& = delete;

    auto genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto estFlottant() const -> bool override;
    [[nodiscard]] auto estBooleen() const -> bool override;
    [[nodiscard]] auto estChaine() const -> bool override;
    [[nodiscard]] auto estTableau() const -> bool override { return true; }

    [[nodiscard]] auto getTypeEnfant() const -> IType* { return _typeEnfant; }
    [[nodiscard]] auto getTaille() const -> INoeud* { return _taille; }
};

#endif /* B05A551C_2519_458F_BB1D_8E5EF8DB9B83 */
