#ifndef EXPRESSION_UNREFVARIABLE_CPP
#define EXPRESSION_UNREFVARIABLE_CPP

#include "compiler/variable/expression_un_ref_variable.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <stdexcept>
#include <vector>

ExpressionUnRefVariable::ExpressionUnRefVariable(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionUnRefVariable::~ExpressionUnRefVariable()
= default;

auto ExpressionUnRefVariable::build(std::vector<Token>& equation) -> INode*
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIER) {
        throw std::runtime_error("Error: 'unref' must be followed by an identifier");
    }

    std::size_t node_id = _context.getNodeComponentRegistry()->getNextId();
    _context.getNodeComponentRegistry()->insert<AST_NAME_COMPONENT>(node_id, equation[1]);
    return _context.getBuilderTreeEquation()->allocate<NodeUnRefVariable>(node_id); 
}

#endif /* EXPRESSION_UNREFVARIABLE_CPP */