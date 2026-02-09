#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/GestionnaireErreur.h"
#include <memory>
#include <stdexcept>

Token ParseurBase::consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageErreur)
{
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[index].type != typeAttendu) {
        if (index >= 0 && index < static_cast<int>(tokens.size())) {
            throw ErreurCompilation(messageErreur, tokens[index].ligne, tokens[index].colonne);
        } else {
            throw ErreurCompilation(messageErreur, 1, 1);
        }
    }
    return tokens[index++];
}

void ParseurBase::consommerEnfantCorps(std::vector<Token>& tokens, int& index , const std::shared_ptr<IInstruction>& parent, ConstructeurArbreInstruction* constructeurArbreInstruction,TokenType fin)
{
    while(index < (int)tokens.size() && tokens[index].type != fin)
    {
        // Ignorer les virgules entre les éléments
        if(tokens[index].type == TOKEN_VIRGULE)
        {
            index++;
            continue;
        }
        
        std::shared_ptr<INoeud> enfant = constructeurArbreInstruction->construire(tokens, index);
        parent->ajouterInstruction(enfant);
    }
}
