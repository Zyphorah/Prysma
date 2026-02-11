#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>


ParseurAppelFonction::ParseurAppelFonction()
{
}

ParseurAppelFonction::~ParseurAppelFonction()
{
    
}

//call nomFonction()
std::shared_ptr<INoeud> ParseurAppelFonction::parser(std::vector<Token>& tokens, int& index, [[maybe_unused]] IConstructeurArbre* constructeurArbre) 
{

    consommer(tokens,index,TOKEN_CALL,"Erreur: ce n'est pas le token call, la fonction est invalid! 'call'");
    Token nomFonction = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: l'identifiant est invalid");
    std::shared_ptr<IInstruction> parent = make_shared<NoeudAppelFonction>(nomFonction);
    consommer(tokens,index,TOKEN_PAREN_OUVERTE, "Erreur: le token est invalide!'('");
    
    auto builder = std::make_shared<ConstructeurEquationFlottante>();
    
    while(index < (int)tokens.size() && tokens[index].type != TOKEN_PAREN_FERMEE)
    {
        size_t debutArg = index;
        size_t finArg = index;
        
        int profondeurParentheses = 0;
        while(finArg < tokens.size())
        {
            if(tokens[finArg].type == TOKEN_PAREN_OUVERTE) {
                profondeurParentheses++;
            }
            else if(tokens[finArg].type == TOKEN_PAREN_FERMEE) {
                if(profondeurParentheses == 0) {
                    break;
                }
                profondeurParentheses--;
            }
            else if(tokens[finArg].type == TOKEN_VIRGULE && profondeurParentheses == 0) {
                break;
            }
            finArg++;
        }
        
        std::vector<Token> tokensArg(tokens.begin() + debutArg, tokens.begin() + finArg);
        std::shared_ptr<INoeud> argument = builder->construire(tokensArg);
        parent->ajouterInstruction(argument);
        
        index = finArg;
        if(index < (int)tokens.size() && tokens[index].type == TOKEN_VIRGULE)
        {
            index++;
        }
    }
    
    consommer(tokens,index,TOKEN_PAREN_FERMEE, "Erreur: le token est invalide!')'");
    consommer(tokens,index,TOKEN_POINT_VIRGULE, "Erreur: le token est invalide!';'");

    return parent; 
}