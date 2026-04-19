#ifndef EXPRESSION_STRING_CPP
#define EXPRESSION_STRING_CPP

#include "compiler/math/expression_string.h"
#include "../../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstring>
#include <llvm/ADT/SmallVector.h>
#include <cstddef>
#include <llvm/ADT/StringRef.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>

ExpressionString::ExpressionString(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionString::~ExpressionString()
= default;

auto ExpressionString::build(std::vector<Token>& equation) -> INode*
{
    llvm::SmallVector<INode*, 16> stringElements;

    std::size_t index = 0;
    if (equation.empty() || equation[0].type != TOKEN_QUOTE) {
        throw std::runtime_error("Error: a string must start with a quote");
    }
    index++;

    if (index >= equation.size() || equation[index].type != TOKEN_IDENTIFIER) {
        throw std::runtime_error("Error: a string must be composed of alphanumeric characters");
    }
    Token str = equation[index];
    index++;

    if (index >= equation.size() || equation[index].type != TOKEN_QUOTE) {
        throw std::runtime_error("Error: a string must end with a quote");
    }

    for (std::size_t charIndex = 0; charIndex < str.value.size(); charIndex++) {
        int ascii = static_cast<unsigned char>(str.value[charIndex]);
        Token token;
        token.type = TOKEN_LIT_INT;
        std::string asciiStr = std::to_string(ascii);
        
        auto& arena = _context.getBuilderTreeEquation()->getArena();
        char* arr = static_cast<char*>(arena.Allocate(asciiStr.size() + 1, 1));
        std::memcpy(arr, asciiStr.c_str(), asciiStr.size() + 1);
        
        token.value = llvm::StringRef(arr, asciiStr.size());
        token.line = str.line;
        token.column = str.column;

        auto* new_node_literal = _context.getBuilderTreeEquation()->allocate<NodeArrayInitialization>(
            _context.getNodeComponentRegistry()->getNextId()
        );
        _context.getNodeComponentRegistry()->emplace<NodeLiteralComponents>(new_node_literal->getNodeId(), token);

        stringElements.push_back(new_node_literal); 
    }

    Token tokenZero;
    tokenZero.type = TOKEN_LIT_INT;
    tokenZero.value = "0";
    tokenZero.line = str.line;
    tokenZero.column = str.column;

    auto* new_node_literal = _context.getBuilderTreeEquation()->allocate<NodeArrayInitialization>(
        _context.getNodeComponentRegistry()->getNextId()
    );
    _context.getNodeComponentRegistry()->emplace<NodeLiteralComponents>(new_node_literal->getNodeId(), tokenZero);
    stringElements.push_back(new_node_literal); 

    auto* new_node = _context.getBuilderTreeEquation()->allocate<NodeArrayInitialization>(
        _context.getNodeComponentRegistry()->getNextId()
    );

    _context.getNodeComponentRegistry()->emplace<NodeArrayInitializationComponents>(
        new_node->getNodeId(),
        _context.getBuilderTreeEquation()->allocateArray<INode*>(stringElements)
    );

    return new_node;
}

#endif /* EXPRESSION_STRING_CPP */