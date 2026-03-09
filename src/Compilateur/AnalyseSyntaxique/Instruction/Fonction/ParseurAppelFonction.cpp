#include "Compilateur/AnalyseSyntaxique/ParseurAppelFonction.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/AST_Genere.h"

ParseurAppelFonction::ParseurAppelFonction(ContextParseur& contextParseur) : _contextParseur(contextParseur)
{
}

ParseurAppelFonction::~ParseurAppelFonction()
{
}

INoeud* ParseurAppelFonction::parser(std::vector<Token>& tokens, int& index)
{
    const bool appelCommeInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EGAL;

    consommer(tokens, index, TOKEN_CALL, "Erreur: 'call' attendu");
    Token nomFonction = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant de fonction attendu");
    consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: '(' attendue");
    
    IInstruction* noeudAppel = _contextParseur.constructeurArbreEquation->allouer<NoeudAppelFonction>(nomFonction);
    
    consommerEnfantCorps(tokens, index, noeudAppel, _contextParseur.constructeurArbreEquation, TOKEN_PAREN_FERMEE);

    consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ')' attendue");

    if (appelCommeInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_POINT_VIRGULE) {
        consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'appel de fonction");
    }

    return noeudAppel;
}
