#ifndef F18431EF_5246_41F8_BED6_A8C2574165E4
#define F18431EF_5246_41F8_BED6_A8C2574165E4

#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/macros/prysma_nodiscard.h"
#include "compiler/visitor/visitor_base_generale.h"

class NodeArgFunction;

// Extracts an argument node from a function argument node
class ArgExtractorFunction : public VisitorBaseGenerale {
public:
    explicit ArgExtractorFunction(ContextGenCode* contextGenCode);
    ~ArgExtractorFunction() override;

    ArgExtractorFunction(const ArgExtractorFunction&) = delete;
    auto operator=(const ArgExtractorFunction&) -> ArgExtractorFunction& = delete;
    ArgExtractorFunction(ArgExtractorFunction&&) = delete;
    auto operator=(ArgExtractorFunction&&) -> ArgExtractorFunction& = delete;

public:
    using VisitorBaseGenerale::visiter;

    void visiter(NodeArgFunction* node) override;
    PRYSMA_NODISCARD auto getArg() const -> NodeArgFunction* { return arg; }

private:
    NodeArgFunction* arg = nullptr;
    ContextGenCode* _contextGenCode; 

};

#endif /* F18431EF_5246_41F8_BED6_A8C2574165E4 */