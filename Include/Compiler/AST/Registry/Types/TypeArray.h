#ifndef B05A551C_2519_458F_BB1D_8E5EF8DB9B83
#define B05A551C_2519_458F_BB1D_8E5EF8DB9B83

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "IType.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

class TypeArray : public IType {
private:
    IType* _childType;
    INode* _size; 

public:
    TypeArray(IType* childType, INode* size);
    ~TypeArray() override = default;

    TypeArray(const TypeArray&) = delete;
    auto operator=(const TypeArray&) -> TypeArray& = delete;
    TypeArray(TypeArray&&) = delete;
    auto operator=(TypeArray&&) -> TypeArray& = delete;

    auto generateLLVMType(llvm::LLVMContext& context) -> llvm::Type* override;
    
    [[nodiscard]] auto isFloating() const -> bool override;
    [[nodiscard]] auto isBoolean() const -> bool override;
    [[nodiscard]] auto isString() const -> bool override;
    [[nodiscard]] auto isArray() const -> bool override { return true; }

    [[nodiscard]] static bool classof(const IType* type) { return type->isArray(); }

    [[nodiscard]] auto getChildType() const -> IType* { return _childType; }
    [[nodiscard]] auto getSize() const -> INode* { return _size; }
};

#endif /* B05A551C_2519_458F_BB1D_8E5EF8DB9B83 */
