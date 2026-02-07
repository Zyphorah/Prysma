#include "Compilateur/Parsing/ParserBase.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
#include <stdexcept>

Token ParserBase::consommer(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageErreur)
{
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[index].type != typeAttendu) {
        throw std::runtime_error(messageErreur);
    }
    return tokens[index++];
}

void ParserBase::consommerEnfantCorps(std::vector<Token>& tokens, int& index , const std::shared_ptr<IInstruction>& parent, ConstructeurArbreInstruction* constructeurArbreInstruction,TokenType fin)
{
    while(index < (int)tokens.size() && tokens[index].type != fin)
    {
        std::shared_ptr<INoeud> enfant = constructeurArbreInstruction->construire(tokens, index);
        parent->ajouterInstruction(enfant);
    }
}
