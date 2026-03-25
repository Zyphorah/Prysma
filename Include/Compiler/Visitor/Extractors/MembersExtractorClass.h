#ifndef D3A3E436_A6F0_4651_9DE8_DD58645AE33C
#define D3A3E436_A6F0_4651_9DE8_DD58645AE33C

#include "Compiler/Visitor/VisitorBaseGenerale.h"
#include <vector>
#include <string>

class NodeDeclarationFunction;
class NodeDeclarationVariable;
class NodeClass;

class MembersExtractorClass : public VisitorBaseGenerale {
private:
    std::vector<NodeDeclarationFunction*> methodes;
    std::vector<NodeDeclarationVariable*> variables;
    std::string nomClass;

public:
    [[nodiscard]] auto getMethodes() const -> const std::vector<NodeDeclarationFunction*>& { return methodes; }
    [[nodiscard]] auto getVariables() const -> const std::vector<NodeDeclarationVariable*>& { return variables; }
    [[nodiscard]] auto getNomClass() const -> const std::string& { return nomClass; }
    using VisitorBaseGenerale::visiter;
    void visiter(NodeDeclarationFunction* node) override;
    void visiter(NodeDeclarationVariable* node) override;
    void visiter(NodeClass* node) override;
};

#endif /* D3A3E436_A6F0_4651_9DE8_DD58645AE33C */
