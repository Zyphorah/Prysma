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

ParserType::ParserType(RegistryType* registryType, IBuilderTree* builderTree)
    : _registryType(registryType), _builderTree(builderTree)
{
}

auto ParserType::parser(std::vector<Token>& tokens, int& index) -> IType*
{
    // Vérifier que le token courant est un type valide
    if (!estType(tokens[static_cast<size_t>(index)].type)) {
        throw ErrorCompilation("Error : type attendu", Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
    }

    IType* type = nullptr;
    
    if (tokens[static_cast<size_t>(index)].type == TOKEN_IDENTIFIANT) {
        type = _builderTree->allouer<TypeComplex>(tokens[static_cast<size_t>(index)].value);
    } else {
        llvm::Type* typeLLVM = _registryType->recuperer(tokens[static_cast<size_t>(index)].type);
        type = _builderTree->allouer<TypeSimple>(typeLLVM);
    }
    index++;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_CROCHET_OUVERT) {
    
        index++; // Consommer le crochet ouvert

        INode* tailleEquation = nullptr;
        
        // Vérifier si la taille est spécifiée ou si les crochets sont vides
        if (tokens[static_cast<size_t>(index)].type != TOKEN_CROCHET_FERME) {
            tailleEquation = _builderTree->construire(tokens, index);
        }

        if (index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != TOKEN_CROCHET_FERME) {
            throw ErrorCompilation("Error : ']' attendu après la taille du array", Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
        }
        index++; // Consommer le crochet fermant

        type = _builderTree->allouer<TypeArray>(type, tailleEquation);
    }
    return type;
}
