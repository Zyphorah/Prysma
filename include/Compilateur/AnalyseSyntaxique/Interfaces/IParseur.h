#ifndef A32B33F7_0073_416C_873A_6FF7F40419F8
#define A32B33F7_0073_416C_873A_6FF7F40419F8

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>

class IParseur
{
public: 
    virtual ~IParseur() = default;
    virtual INoeud* parser(std::vector<Token>& tokens, int& index) = 0;
};

#endif /* A32B33F7_0073_416C_873A_6FF7F40419F8 */
