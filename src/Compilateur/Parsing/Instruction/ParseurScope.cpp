#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/ParserScope.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/NoeudScope.h"

std::shared_ptr<INoeud> ParserScope::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    std::shared_ptr<NoeudScope> noeudScope = std::make_shared<NoeudScope>();
    consommer(tokens, index, TOKEN_SCOPE, "Erreur : 'scope' attendu");
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur : '{' attendu après 'scope'");

    // faire un throw exception : || index == (int)tokens.size() cela veux dire qu'il n'y a pas de valeur 
    
    while(index < (int)tokens.size() && tokens[index].type != TOKEN_ACCOLADE_FERMEE)
    {
        std::shared_ptr<INoeud> enfant = constructeurArbreInstruction->construire(tokens, index);
        noeudScope->ajouterInstruction(enfant);
    }
     
    return noeudScope;
}