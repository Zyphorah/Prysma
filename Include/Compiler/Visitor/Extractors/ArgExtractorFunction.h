#ifndef F18431EF_5246_41F8_BED6_A8C2574165E4
#define F18431EF_5246_41F8_BED6_A8C2574165E4

#include "Compiler/Visitor/VisitorBaseGenerale.h"

class NodeArgFunction;

class ArgExtractorFunction : public VisitorBaseGenerale {
public:
    using VisitorBaseGenerale::visiter;
    void visiter(NodeArgFunction* node) override;
    [[nodiscard]] auto getArg() const -> NodeArgFunction* { return arg; }

private:
    NodeArgFunction* arg = nullptr;
};

#endif /* F18431EF_5246_41F8_BED6_A8C2574165E4 */
