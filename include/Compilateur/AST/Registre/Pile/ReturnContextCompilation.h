#ifndef D5D5CC5E_96E0_4410_95A8_57E0E7660888
#define D5D5CC5E_96E0_4410_95A8_57E0E7660888

#include "Compilateur/Lexer/TokenType.h"
#include <stack>
class ReturnContextCompilation
{
private:
    std::stack<TokenType> _contexte; 

public:

    ReturnContextCompilation()
    {}

    ~ReturnContextCompilation()
    {}

    TokenType recupererContext();
        
    void piler(const TokenType& token);
    void depiler();

};

#endif /* D5D5CC5E_96E0_4410_95A8_57E0E7660888 */
