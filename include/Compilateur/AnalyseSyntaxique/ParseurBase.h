#ifndef DD335087_6EDE_4036_872C_8BD586E26251
#define DD335087_6EDE_4036_872C_8BD586E26251

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>
#include <string>

class ParseurBase
{
protected:
    static Token consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageErreur);
    static void consommerEnfantCorps(std::vector<Token>& tokens, int& index, IInstruction* parent, IConstructeurArbre* constructeurArbre,TokenType fin);
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26251 */
