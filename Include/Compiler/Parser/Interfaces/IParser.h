#ifndef A32B33F7_0073_416C_873A_6FF7F40419F8
#define A32B33F7_0073_416C_873A_6FF7F40419F8

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class IParser
{
public: 
    IParser() = default;
    virtual ~IParser() = default;
    IParser(const IParser&) = delete;
    auto operator=(const IParser&) -> IParser& = delete;
    IParser(IParser&&) = delete;
    auto operator=(IParser&&) -> IParser& = delete;
    virtual auto parser(std::vector<Token>& tokens, int& index) -> INode* = 0;
};

#endif /* A32B33F7_0073_416C_873A_6FF7F40419F8 */
