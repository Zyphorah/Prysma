#ifndef EXPRESSION_REFVARIABLE_CPP
#define EXPRESSION_REFVARIABLE_CPP

#include "compiler/variable/expression_ref_variable.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <stdexcept>
#include <vector>

ExpressionRefVariable::ExpressionRefVariable(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionRefVariable::~ExpressionRefVariable()
= default;

auto ExpressionRefVariable::build(std::vector<Token>& equation) -> INode*
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIER) {
        throw std::runtime_error("Error: 'ref' must be followed by an identifier");
    }

    auto* new_node = _context.getBuilderTreeEquation()->allocate<NodeRefVariable>(
        _context.getNodeComponentRegistry()->getNextId()
    );

    _context.getNodeComponentRegistry()->insert<AST_NAME_COMPONENT>(
        new_node->getNodeId(),
        equation[1]
    );

    return new_node;
}

#endif /* EXPRESSION_REFVARIABLE_CPP */