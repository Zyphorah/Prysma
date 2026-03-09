#include "Compilateur/AnalyseSyntaxique/ParseurAffectationVariable.h"
#include "Compilateur/AST/AST_Genere.h"


ParseurAffectationVariable::ParseurAffectationVariable(ContextParseur& contextParseur)
    : _contextParseur(contextParseur)
{
}

ParseurAffectationVariable::~ParseurAffectationVariable()
{
}

INoeud* ParseurAffectationVariable::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_AFF, "Erreur : 'aff' attendu");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom variable attendu");
    std::string nomVariable = nomToken.value;

    INoeud* expressionIndex = nullptr;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_CROCHET_OUVERT) {
 
        consommer(tokens, index, TOKEN_CROCHET_OUVERT, "Erreur '['");
        expressionIndex = _contextParseur.constructeurArbreEquation->construire(tokens, index);
        consommer(tokens, index, TOKEN_CROCHET_FERME, "Erreur : ']' attendu après l'index");
    }

    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    INoeud* expression = _contextParseur.constructeurArbreEquation->construire(tokens, index);
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu");

    if (expressionIndex != nullptr) {
        return _contextParseur.constructeurArbreEquation->allouer<NoeudAffectationTableau>(nomVariable, expressionIndex, expression, nomToken);
    }
    
    return _contextParseur.constructeurArbreEquation->allouer<NoeudAffectationVariable>(nomVariable, expression, nomToken);
}