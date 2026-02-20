#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
ParseurIf::ParseurIf(IConstructeurArbre* constructeurArbreEquation, IConstructeurArbre* constructeurArbreInstruction) 
    : _constructeurArbreEquation(constructeurArbreEquation), _constructeurArbreInstruction(constructeurArbreInstruction)
{

}

ParseurIf::~ParseurIf()
{}

INoeud* ParseurIf::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens,index,TOKEN_SI,"Erreur, le token n'est pas 'if'! ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
    
  
    INoeud* condition = _constructeurArbreEquation->construire(tokens, index);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

    // Créer le noeud bloc IF
    NoeudInstruction* noeudBlocIf = new NoeudInstruction();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudBlocIf,_constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");

    // Créer le noeud bloc ELSE s'il existe
    NoeudInstruction* noeudBlocElse = nullptr;
    if (index < (int)tokens.size() && tokens[index].type == TOKEN_SINON) {
        consommer(tokens,index,TOKEN_SINON,"Erreur, le token n'est pas 'else'! ");
        noeudBlocElse = new NoeudInstruction();
        consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
        consommerEnfantCorps(tokens,index,noeudBlocElse,_constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
        consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");
    }

    // Créer le noeud bloc ENDIF
    NoeudInstruction* noeudBlocEndif = new NoeudInstruction();

    NoeudIf* noeudIf = new NoeudIf(condition, noeudBlocIf, noeudBlocElse, noeudBlocEndif);

    return noeudIf;
}