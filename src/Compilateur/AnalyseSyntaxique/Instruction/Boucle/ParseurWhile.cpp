#include "Compilateur/AnalyseSyntaxique/Instruction/Boucle/ParseurWhile.h"
#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurWhile::ParseurWhile(IConstructeurArbre* constructeurArbreEquation, IConstructeurArbre* constructeurArbreInstruction) 
    : _constructeurArbreEquation(constructeurArbreEquation), _constructeurArbreInstruction(constructeurArbreInstruction)
{}

ParseurWhile::~ParseurWhile()
{}

// exemple : while()
//{}

std::shared_ptr<INoeud> ParseurWhile::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens,index,TOKEN_TANT_QUE,"Erreur, token attendu 'while' ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
    
    std::shared_ptr<INoeud> condition = _constructeurArbreEquation->construire(tokens, index);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

    std::shared_ptr<NoeudInstruction> noeudBlocWhile = std::make_shared<NoeudInstruction>();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudBlocWhile,_constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");


    std::shared_ptr<NoeudInstruction> noeudBlocEndWhile = std::make_shared<NoeudInstruction>();

    std::shared_ptr<NoeudWhile> noeudWhile = std::make_shared<NoeudWhile>(std::move(condition), std::move(noeudBlocWhile), std::move(noeudBlocEndWhile));

    return noeudWhile;
}