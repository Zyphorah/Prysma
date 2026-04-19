#ifndef PARSER_DECLARATIONFUNCTION_CPP
#define PARSER_DECLARATIONFUNCTION_CPP

#include "compiler/function/parser_declaration_function.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <llvm/ADT/SmallVector.h>
#include <cstddef>
#include <string>
#include <vector>


ParserDeclarationFunction::ParserDeclarationFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDeclarationFunction::~ParserDeclarationFunction() = default;

auto ParserDeclarationFunction::parse(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    consume(tokens, index, TOKEN_FUNCTION, "Error: not the correct token! 'fn'");

    Token tokenTypeReturn = tokens[index];
    IType* typeReturn = _contextParser.getTypeParser()->parse(tokens, index);
    
    Token tokenFunctionName = tokens[index];
    consume(tokens, index, TOKEN_IDENTIFIER, "Error: invalid identifier, must be a function name");

    // Parse arguments inside parentheses
    consume(tokens, index, TOKEN_PAREN_OPEN, "Error: not an opening parenthesis '('");
    
    llvm::SmallVector<INode*, 4> arguments;
    while(index < tokens.size() && tokens[index].type != TOKEN_PAREN_CLOSE)
    {
        if(tokens[index].type == TOKEN_COMMA)
        {
            index++;
            continue;
        }
        
        INode* child = _contextParser.getBuilderTreeInstruction()->build(tokens, index);
        arguments.push_back(child);
    }

    consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: not a closing parenthesis ')'");

    // Parse the body in a strict NodeScope
    consume(tokens, index, TOKEN_BRACE_OPEN, "Error: not an opening brace '{' ");


    auto bodyChildren = consumeChildBody(tokens, index, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);

    auto* body_node = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    ); 
    _contextParser.getNodeComponentRegistry()->emplace<NodeInstructionComponents>(body_node->getNodeId(), bodyChildren);


    consume(tokens, index, TOKEN_BRACE_CLOSE, "Error: not a closing brace '}'");

    auto* nodeFunction = 
        _contextParser.getBuilderTreeInstruction()->allocate<NodeDeclarationFunction>(
            Token{}, typeReturn, tokenFunctionName, _contextParser.getBuilderTreeInstruction()->allocateArray<INode*>(
                arguments
            ), body_node
        );

    return nodeFunction; 
}

#endif /* PARSER_DECLARATIONFUNCTION_CPP */




