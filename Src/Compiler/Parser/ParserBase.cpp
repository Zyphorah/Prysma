#include "Compiler/Parser/ParserBase.h"
#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/ManagerError.h"
#include <cstddef>
#include <string>
#include <vector>

auto ParserBase::consume(std::vector<Token>& tokens, int& index, TokenType expectedType, const std::string& errorMessage) -> Token
{
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != expectedType) {
        if (index >= 0 && index < static_cast<int>(tokens.size())) {
            throw CompilationError(errorMessage, Line(tokens[static_cast<size_t>(index)].line), Column(tokens[static_cast<size_t>(index)].column));
        }
        throw CompilationError(errorMessage, Line(1), Column(1));
    }
    return tokens[static_cast<size_t>(index++)];
}

void ParserBase::consumeChildBody(std::vector<Token>& tokens, int& index , IInstruction* parent, IBuilderTree* builderTree, TokenType end)
{
    while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != end)
    {
        // Ignore commas between elements
        if(tokens[static_cast<size_t>(index)].type == TOKEN_COMMA)
        {
            index++;
            continue;
        }
        
        INode* child = builderTree->build(tokens, index);
        parent->addInstruction(child);
    }
}
