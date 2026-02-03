#ifndef DD335087_6EDE_4036_872C_8BD586E26251
#define DD335087_6EDE_4036_872C_8BD586E26251

#include "Compilateur/Lexer/Lexer.h"
#include <vector>
#include <string>

class ParserBase
{
protected:
    Token consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageErreur);
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26251 */
