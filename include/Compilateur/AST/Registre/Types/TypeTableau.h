#ifndef B05A551C_2519_458F_BB1D_8E5EF8DB9B83
#define B05A551C_2519_458F_BB1D_8E5EF8DB9B83

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "IType.h"
#include <memory>

class TypeTableau : public IType {
private:
    IType* _typeEnfant;
    INoeud* _taille; 

public:
    TypeTableau(IType* typeEnfant, INoeud* taille);
    ~TypeTableau() override = default;

    llvm::Type* genererTypeLLVM(llvm::LLVMContext& context) override;

    IType* getTypeEnfant() const { return _typeEnfant; }
    INoeud* getTaille() const { return _taille; }
};

#endif /* B05A551C_2519_458F_BB1D_8E5EF8DB9B83 */
