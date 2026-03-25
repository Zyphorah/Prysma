#ifndef B05A551C_2519_458F_BB1D_8E5EF8DB9B83
#define B05A551C_2519_458F_BB1D_8E5EF8DB9B83

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "IType.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

class TypeArray : public IType {
private:
    IType* _typeChild;
    INode* _taille; 

public:
    TypeArray(IType* typeChild, INode* taille);
    ~TypeArray() override = default;

    TypeArray(const TypeArray&) = delete;
    auto operator=(const TypeArray&) -> TypeArray& = delete;
    TypeArray(TypeArray&&) = delete;
    auto operator=(TypeArray&&) -> TypeArray& = delete;

    auto generatedrTypeLLVM(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto estFlottant() const -> bool override;
    [[nodiscard]] auto estBooleen() const -> bool override;
    [[nodiscard]] auto estChaine() const -> bool override;
    [[nodiscard]] auto estArray() const -> bool override { return true; }

    [[nodiscard]] static bool classof(const IType* type) { return type->estArray(); }

    [[nodiscard]] auto getTypeChild() const -> IType* { return _typeChild; }
    [[nodiscard]] auto getTaille() const -> INode* { return _taille; }
};

#endif /* B05A551C_2519_458F_BB1D_8E5EF8DB9B83 */
