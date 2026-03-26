#ifndef BA12D61E_9205_4816_94C1_379B8ABA63D3
#define BA12D61E_9205_4816_94C1_379B8ABA63D3

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class TypeParser {
private:
    RegistryType* _registryType;
    IBuilderTree* _builderTree;

public:
    TypeParser(RegistryType* registryType, IBuilderTree* builderTree);
    ~TypeParser() = default;

    TypeParser(const TypeParser&) = delete;
    auto operator=(const TypeParser&) -> TypeParser& = delete;
    TypeParser(TypeParser&&) = delete;
    auto operator=(TypeParser&&) -> TypeParser& = delete;
    auto parse(std::vector<Token>& tokens, int& index) -> IType*;
};

#endif /* BA12D61E_9205_4816_94C1_379B8ABA63D3 */
