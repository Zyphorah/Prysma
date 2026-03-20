#ifndef D5D5CC5E_96E0_4410_95A8_57E0E7660888
#define D5D5CC5E_96E0_4410_95A8_57E0E7660888

#include "Compilateur/Lexer/TokenType.h"
#include <stack>
class ReturnContextCompilation
{
private:
    std::stack<TokenType> _contexte; 

public:

    ReturnContextCompilation() = default;
    ReturnContextCompilation(const ReturnContextCompilation&) = default;
    ReturnContextCompilation(ReturnContextCompilation&&)  noexcept = default;
    auto operator=(const ReturnContextCompilation&) -> ReturnContextCompilation& = default;
    auto operator=(ReturnContextCompilation&&) -> ReturnContextCompilation& = default;
    ~ReturnContextCompilation() = default;

    auto recupererContext() -> TokenType;
        
    void piler(const TokenType& token);
    void depiler();

};

#endif /* D5D5CC5E_96E0_4410_95A8_57E0E7660888 */
