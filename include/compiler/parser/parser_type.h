#ifndef BA12D61E_9205_4816_94C1_379B8ABA63D3
#define BA12D61E_9205_4816_94C1_379B8ABA63D3

#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/ast/registry/registry_type.h"
#include "compiler/lexer/lexer.h"
#include <cstddef>
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
    auto parse(std::vector<Token>& tokens, std::size_t index) -> IType*;
};

#endif /* BA12D61E_9205_4816_94C1_379B8ABA63D3 */
