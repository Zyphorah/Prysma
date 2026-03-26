#include "Compiler/Parser/ParserType.h"
#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include "Compiler/AST/Registry/Types/TypeArray.h"
#include "Compiler/AST/Registry/Types/TypeComplex.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenCategories.h"
#include "Compiler/ManagerError.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <llvm/IR/Type.h>
#include <vector>

TypeParser::TypeParser(RegistryType* registryType, IBuilderTree* builderTree)
    : _registryType(registryType), _builderTree(builderTree)
{
}

auto TypeParser::parse(std::vector<Token>& tokens, int& index) -> IType*
{
    // Check that the current token is a valid type
    if (!isType(tokens[static_cast<size_t>(index)].type)) {
        throw CompilationError("Error: expected type", Line(tokens[static_cast<size_t>(index)].line), Column(tokens[static_cast<size_t>(index)].column));
    }

    IType* type = nullptr;
    
    if (tokens[static_cast<size_t>(index)].type == TOKEN_IDENTIFIER) {
        type = _builderTree->allocate<TypeComplex>(tokens[static_cast<size_t>(index)].value);
    } else {
        llvm::Type* typeLLVM = _registryType->get(tokens[static_cast<size_t>(index)].type);
        type = _builderTree->allocate<TypeSimple>(typeLLVM);
    }
    index++;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_BRACKET_OPEN) {
    
        index++; // Consume the opening bracket

        INode* sizeEquation = nullptr;
        
        // Check if the size is specified or if the brackets are empty
        if (tokens[static_cast<size_t>(index)].type != TOKEN_BRACKET_CLOSE) {
            sizeEquation = _builderTree->build(tokens, index);
        }

        if (index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != TOKEN_BRACKET_CLOSE) {
            throw CompilationError("Error: ']' expected after array size", Line(tokens[static_cast<size_t>(index)].line), Column(tokens[static_cast<size_t>(index)].column));
        }
        index++; // Consume the closing bracket

        type = _builderTree->allocate<TypeArray>(type, sizeEquation);
    }
    return type;
}
