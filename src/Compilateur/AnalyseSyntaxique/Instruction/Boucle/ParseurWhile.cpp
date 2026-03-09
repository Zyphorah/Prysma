#include "Compilateur/AnalyseSyntaxique/ParseurWhile.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurWhile::ParseurWhile(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurWhile::~ParseurWhile()
{}

// exemple : while()
//{}

INoeud* ParseurWhile::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens,index,TOKEN_TANT_QUE,"Erreur, token attendu 'while' ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
    
    INoeud* condition = _contextParseur.constructeurArbreEquation->construire(tokens, index);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

    IInstruction* noeudBlocWhile = _contextParseur.constructeurArbreInstruction->allouer<NoeudInstruction>();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudBlocWhile,_contextParseur.constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");


    IInstruction* noeudBlocEndWhile = _contextParseur.constructeurArbreInstruction->allouer<NoeudInstruction>();

    IInstruction* noeudWhile = _contextParseur.constructeurArbreInstruction->allouer<NoeudWhile>(condition, noeudBlocWhile, noeudBlocEndWhile);

    return noeudWhile;
}