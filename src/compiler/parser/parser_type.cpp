#include "compiler/parser/parser_type.h"
#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/ast/registry/registry_type.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/ast/registry/types/type_simple.h"
#include "compiler/ast/registry/types/type_array.h"
#include "compiler/ast/registry/types/type_complex.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_categories.h"
#include "compiler/manager_error.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <llvm/IR/Type.h>
#include <vector>

TypeParser::TypeParser(RegistryType* registryType, NodeComponentRegistry* nodeComponentRegistry, IBuilderTree* builderTree)
    : _registryType(registryType), _builderTree(builderTree), _nodeComponentRegistry(nodeComponentRegistry)
{
}

auto TypeParser::parse(std::vector<Token>& tokens, std::size_t index) -> IType*
{
    // Check that the current token is a valid type
    if (!isType(tokens[index].type)) {
        throw CompilationError("Error: expected type", Line(tokens[index].line), Column(tokens[index].column));
    }

    IType* type = nullptr;
    
    if (tokens[index].type == TOKEN_IDENTIFIER) {
        type = _builderTree->allocate<TypeComplex>(std::string(tokens[index].value));
    } else {
        llvm::Type* typeLLVM = _registryType->get(tokens[index].type);
        type = _builderTree->allocate<TypeSimple>(typeLLVM);
    }
    index++;

    if (index < tokens.size() && tokens[index].type == TOKEN_BRACKET_OPEN) {
    
        index++; // Consume the opening bracket

        INode* sizeEquation = nullptr;
        
        // Check if the size is specified or if the brackets are empty
        if (tokens[index].type != TOKEN_BRACKET_CLOSE) {
            sizeEquation = _builderTree->build(tokens, index);
        }

        if (index >= tokens.size() || tokens[index].type != TOKEN_BRACKET_CLOSE) {
            throw CompilationError("Error: ']' expected after array size", Line(tokens[index].line), Column(tokens[index].column));
        }
        index++; // Consume the closing bracket

        type = _builderTree->allocate<TypeArray>(type, sizeEquation, _nodeComponentRegistry);
    }
    return type;
}