#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/ParserMain.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Noeuds/NoeudMain.h"

std::shared_ptr<INoeud> ParserMain::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    std::shared_ptr<NoeudMain> noeudMain = std::make_shared<NoeudMain>();
    consommer(tokens, index, TOKEN_MAIN, "Erreur : 'main' attendu");
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur : '{' attendu après 'main'");

    // faire un throw exception : || index == (int)tokens.size() cela veux dire qu'il n'y a pas de valeur 
    
    while(index < (int)tokens.size() && tokens[index].type != TOKEN_ACCOLADE_FERMEE)
    {
        std::shared_ptr<INoeud> enfant = constructeurArbreInstruction->construire(tokens, index);
        std::vector<std::shared_ptr<INoeud>> enfants;
        enfants.push_back(enfant);
        noeudMain->ajouterInstruction(enfants);
    }
     
    return noeudMain;
}