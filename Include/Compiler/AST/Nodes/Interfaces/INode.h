#pragma once

#include <llvm/IR/Value.h>

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
    
    [[nodiscard]] virtual auto getGeneratedType() const -> NodeTypeGenerated = 0;
};
