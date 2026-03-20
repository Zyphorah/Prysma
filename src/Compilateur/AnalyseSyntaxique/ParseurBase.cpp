#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/GestionnaireErreur.h"
#include <cstddef>
#include <string>
#include <vector>

auto ParseurBase::consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageErreur) -> Token
{
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != typeAttendu) {
        if (index >= 0 && index < static_cast<int>(tokens.size())) {
            throw ErreurCompilation(messageErreur, Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
        }
        throw ErreurCompilation(messageErreur, Ligne(1), Colonne(1));
    }
    return tokens[static_cast<size_t>(index++)];
}

void ParseurBase::consommerEnfantCorps(std::vector<Token>& tokens, int& index , IInstruction* parent, IConstructeurArbre* constructeurArbre,TokenType fin)
{
    while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != fin)
    {
        // Ignorer les virgules entre les éléments
        if(tokens[static_cast<size_t>(index)].type == TOKEN_VIRGULE)
        {
            index++;
            continue;
        }
        
        INoeud* enfant = constructeurArbre->construire(tokens, index);
        parent->ajouterInstruction(enfant);
    }
}
