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
    static auto consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageError) -> Token;
    static void consommerChildBody(std::vector<Token>& tokens, int& index, IInstruction* parent, IBuilderTree* builderTree,TokenType fin);
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26251 */
