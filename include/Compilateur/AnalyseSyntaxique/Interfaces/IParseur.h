#ifndef A32B33F7_0073_416C_873A_6FF7F40419F8
#define A32B33F7_0073_416C_873A_6FF7F40419F8

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class IParseur
{
public: 
    IParseur() = default;
    virtual ~IParseur() = default;
    IParseur(const IParseur&) = delete;
    auto operator=(const IParseur&) -> IParseur& = delete;
    IParseur(IParseur&&) = delete;
    auto operator=(IParseur&&) -> IParseur& = delete;
    virtual auto parser(std::vector<Token>& tokens, int& index) -> INoeud* = 0;
};

#endif /* A32B33F7_0073_416C_873A_6FF7F40419F8 */
