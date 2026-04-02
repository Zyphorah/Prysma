#pragma once

#include <llvm/IR/Value.h>
#include "compiler/macros/prysma_nodiscard.h"

class IVisitor;
enum class NodeTypeGenerated;

class INode {
public:
    INode() = default;
    virtual ~INode() = default;

    INode(const INode&) = delete;
    auto operator=(const INode&) -> INode& = delete;
    INode(INode&&) = delete;
    auto operator=(INode&&) -> INode& = delete;

    virtual void accept(IVisitor* visitor) = 0;
    
    PRYSMA_NODISCARD virtual auto getGeneratedType() const -> NodeTypeGenerated = 0;
};
