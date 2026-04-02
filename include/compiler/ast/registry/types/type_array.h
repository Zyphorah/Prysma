#ifndef B05A551C_2519_458F_BB1D_8E5EF8DB9B83
#define B05A551C_2519_458F_BB1D_8E5EF8DB9B83

#include "compiler/macros/prysma_nodiscard.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "i_type.h"
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
    
    PRYSMA_NODISCARD auto isFloating() const -> bool override;
    PRYSMA_NODISCARD auto isBoolean() const -> bool override;
    PRYSMA_NODISCARD auto isString() const -> bool override;
    PRYSMA_NODISCARD auto isArray() const -> bool override { return true; }

    PRYSMA_NODISCARD static bool classof(const IType* type) { return type->isArray(); }

    PRYSMA_NODISCARD auto getChildType() const -> IType* { return _childType; }
    PRYSMA_NODISCARD auto getSize() const -> INode* { return _size; }
};

#endif /* B05A551C_2519_458F_BB1D_8E5EF8DB9B83 */
