#ifndef DD335087_6EDE_4036_872C_8BD586E26251
#define DD335087_6EDE_4036_872C_8BD586E26251

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>
#include <string>

class ParserBase
{
protected:
    // Consumes a token of the expected type, throws if not found
    static auto consume(std::vector<Token>& tokens, int& index, TokenType expectedType, const std::string& errorMessage) -> Token;
    // Consumes the child body until the end token
    static void consumeChildBody(std::vector<Token>& tokens, int& index, IInstruction* parent, IBuilderTree* builderTree, TokenType end);
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26251 */
