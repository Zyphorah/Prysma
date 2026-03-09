#include "Compilateur/AnalyseSyntaxique/ParseurIf.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurIf::ParseurIf(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{

}

ParseurIf::~ParseurIf()
{}

INoeud* ParseurIf::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens,index,TOKEN_SI,"Erreur, le token n'est pas 'if'! ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
    
  
    INoeud* condition = _contextParseur.constructeurArbreEquation->construire(tokens, index);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

    // Créer le noeud bloc IF
    NoeudInstruction* noeudBlocIf = _contextParseur.constructeurArbreInstruction->allouer<NoeudInstruction>();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudBlocIf,_contextParseur.constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");

    // Créer le noeud bloc ELSE s'il existe
    NoeudInstruction* noeudBlocElse = nullptr;
    if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SINON) {
        consommer(tokens,index,TOKEN_SINON,"Erreur, le token n'est pas 'else'! ");
        noeudBlocElse = _contextParseur.constructeurArbreInstruction->allouer<NoeudInstruction>();
        consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
        consommerEnfantCorps(tokens,index,noeudBlocElse,_contextParseur.constructeurArbreInstruction,TOKEN_ACCOLADE_FERMEE);
        consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");
    }

    // Créer le noeud bloc ENDIF
    NoeudInstruction* noeudBlocEndif = _contextParseur.constructeurArbreInstruction->allouer<NoeudInstruction>();

    NoeudIf* noeudIf = _contextParseur.constructeurArbreInstruction->allouer<NoeudIf>(condition, noeudBlocIf, noeudBlocElse, noeudBlocEndif);

    return noeudIf;
}