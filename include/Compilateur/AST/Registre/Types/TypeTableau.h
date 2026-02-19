#ifndef B05A551C_2519_458F_BB1D_8E5EF8DB9B83
#define B05A551C_2519_458F_BB1D_8E5EF8DB9B83

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "IType.h"
#include <memory>

class TypeTableau : public IType {
private:
    std::shared_ptr<IType> _typeEnfant;
    std::shared_ptr<INoeud> _taille; 

public:
    TypeTableau(std::shared_ptr<IType> typeEnfant, std::shared_ptr<INoeud> taille);
    ~TypeTableau() override = default;

    llvm::Type* genererTypeLLVM(llvm::LLVMContext& context) override;

    std::shared_ptr<IType> getTypeEnfant() const { return _typeEnfant; }
    std::shared_ptr<INoeud> getTaille() const { return _taille; }
};

#endif /* B05A551C_2519_458F_BB1D_8E5EF8DB9B83 */
