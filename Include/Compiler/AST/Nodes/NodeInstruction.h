#ifndef CE440E6B_95E4_4EC8_8C66_1916C9716A77
#define CE440E6B_95E4_4EC8_8C66_1916C9716A77
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include <vector>

enum class NodeTypeGenere;

class NodeInstruction: public IInstruction
{
private: 
    std::vector<INode*> childs; 
public: 

    NodeInstruction();

    NodeInstruction(const NodeInstruction& other);
    auto operator=(const NodeInstruction& other) -> NodeInstruction&;
    NodeInstruction(NodeInstruction&& other) noexcept;
    auto operator=(NodeInstruction&& other)  noexcept -> NodeInstruction&;
     ~NodeInstruction() override = default;

    void accept(IVisitor* visitor) override;
    void ajouterInstruction(INode* child) override;
    
    [[nodiscard]] auto getTypeGenere() const -> NodeTypeGenere override;
    
    [[nodiscard]] auto getChilds() const -> const std::vector<INode*>& { return childs; }
};

#endif /* CE440E6B_95E4_4EC8_8C66_1916C9716A77 */
