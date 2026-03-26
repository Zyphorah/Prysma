#ifndef CE440E6B_95E4_4EC8_8C66_1916C9716A77
#define CE440E6B_95E4_4EC8_8C66_1916C9716A77
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include <vector>

enum class NodeTypeGenerated;

class NodeInstruction: public IInstruction
{
private: 
    std::vector<INode*> children; 
public: 

    NodeInstruction();

    NodeInstruction(const NodeInstruction& other);
    auto operator=(const NodeInstruction& other) -> NodeInstruction&;
    NodeInstruction(NodeInstruction&& other) noexcept;
    auto operator=(NodeInstruction&& other)  noexcept -> NodeInstruction&;
    ~NodeInstruction() override = default;

    void accept(IVisitor* visitor) override;
    void addInstruction(INode* child) override;
    
    [[nodiscard]] auto getGeneratedType() const -> NodeTypeGenerated override;
    
    [[nodiscard]] auto getChildren() const -> const std::vector<INode*>& { return children; }
};

#endif /* CE440E6B_95E4_4EC8_8C66_1916C9716A77 */
