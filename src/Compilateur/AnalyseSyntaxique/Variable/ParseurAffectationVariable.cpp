#ifndef PARSEUR_AFFECTATIONVARIABLE_CPP
#define PARSEUR_AFFECTATIONVARIABLE_CPP

#include "Compilateur/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <string>
#include <vector>


ParseurAffectationVariable::ParseurAffectationVariable(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurAffectationVariable::~ParseurAffectationVariable()
= default;

auto ParseurAffectationVariable::parser(std::vector<Token>& tokens, int& index) -> INoeud*
{
    consommer(tokens, index, TOKEN_AFF, "Erreur : 'aff' attendu");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom variable attendu");
    std::string nomVariable = nomToken.value;

    INoeud* expressionIndex = nullptr;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_CROCHET_OUVERT) {
 
        consommer(tokens, index, TOKEN_CROCHET_OUVERT, "Erreur '['");
        expressionIndex = _contextParseur.getConstructeurArbreEquation()->construire(tokens, index);
        consommer(tokens, index, TOKEN_CROCHET_FERME, "Erreur : ']' attendu après l'index");
    }

    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    INoeud* expression = _contextParseur.getConstructeurArbreEquation()->construire(tokens, index);
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu");

    if (expressionIndex != nullptr) {
        return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudAffectationTableau>(nomVariable, expressionIndex, expression, nomToken);
    }
    
    return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudAffectationVariable>(nomVariable, expression, nomToken);
}

#endif /* PARSEUR_AFFECTATIONVARIABLE_CPP */




