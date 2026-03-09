#include "Compilateur/AnalyseSyntaxique/ParseurDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>


ParseurDeclarationFonction::ParseurDeclarationFonction(ContextParseur& contextParseur)
    : _contextParseur(contextParseur)
{}

ParseurDeclarationFonction::~ParseurDeclarationFonction()
{
}

INoeud* ParseurDeclarationFonction::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_FONCTION, "Erreur: ce n'est pas le bon token ! 'fn'");

    Token tokenTypeRetour = tokens[static_cast<size_t>(index)];
    IType* typeRetour = _contextParseur.parseurType->parser(tokens, index);
    
    Token tokenNomFonction = tokens[static_cast<size_t>(index)];
    std::string nomFonction = tokenNomFonction.value;
    consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant invalide, ce dois être un nom de fonction ");

    // Parser les arguments entre parenthèses
    consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: ce n'est pas une parenthèse ouverte '('");
    
    std::vector<INoeud*> arguments;
    while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_PAREN_FERMEE)
    {
        if(tokens[static_cast<size_t>(index)].type == TOKEN_VIRGULE)
        {
            index++;
            continue;
        }
        
        INoeud* enfant = _contextParseur.constructeurArbreInstruction->construire(tokens, index);
        arguments.push_back(enfant);
    }

    consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ce n'est pas une parenthèse fermée ')'");

    // Parser le corps dans un NoeudScope strict
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur: ce n'est pas une accolade ouverte '{' ");

    NoeudInstruction* corps = _contextParseur.constructeurArbreInstruction->allouer<NoeudInstruction>();
    consommerEnfantCorps(tokens, index, corps, _contextParseur.constructeurArbreInstruction, TOKEN_ACCOLADE_FERMEE);

    consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Erreur: ce n'est pas une accolade fermée '}'");

    NoeudDeclarationFonction* noeudFonction = 
        _contextParseur.constructeurArbreInstruction->allouer<NoeudDeclarationFonction>(typeRetour, nomFonction, arguments, corps);

    return noeudFonction; 
}