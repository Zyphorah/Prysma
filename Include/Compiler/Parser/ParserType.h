#ifndef BA12D61E_9205_4816_94C1_379B8ABA63D3
#define BA12D61E_9205_4816_94C1_379B8ABA63D3

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class ParserType {
private:
    RegistryType* _registryType;
    IBuilderTree* _builderTree;

public:
     ParserType(RegistryType* registryType, IBuilderTree* builderTree);
    ~ParserType() = default;

    ParserType(const ParserType&) = delete;
    auto operator=(const ParserType&) -> ParserType& = delete;
    ParserType(ParserType&&) = delete;
    auto operator=(ParserType&&) -> ParserType& = delete;
    auto parser(std::vector<Token>& tokens, int& index) -> IType*;
};

#endif /* BA12D61E_9205_4816_94C1_379B8ABA63D3 */
