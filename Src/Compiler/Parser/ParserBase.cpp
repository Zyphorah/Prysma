#include "Compiler/Parser/ParserBase.h"
#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/ManagerError.h"
#include <cstddef>
#include <string>
#include <vector>

auto ParserBase::consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageError) -> Token
{
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != typeAttendu) {
        if (index >= 0 && index < static_cast<int>(tokens.size())) {
            throw ErrorCompilation(messageError, Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
        }
        throw ErrorCompilation(messageError, Ligne(1), Colonne(1));
    }
    return tokens[static_cast<size_t>(index++)];
}

void ParserBase::consommerChildBody(std::vector<Token>& tokens, int& index , IInstruction* parent, IBuilderTree* builderTree,TokenType fin)
{
    while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != fin)
    {
        // Ignorer les virgules input les éléments
        if(tokens[static_cast<size_t>(index)].type == TOKEN_VIRGULE)
        {
            index++;
            continue;
        }
        
        INode* child = builderTree->construire(tokens, index);
        parent->ajouterInstruction(child);
    }
}
