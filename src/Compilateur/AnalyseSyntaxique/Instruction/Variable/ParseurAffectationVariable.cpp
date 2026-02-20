#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include <memory>
#include <utility>

ParseurAffectationVariable::ParseurAffectationVariable(LlvmBackend* backend, RegistreVariable* registreVariable, RegistreType* registreType, IConstructeurArbre* constructeurEquation)
    : _backend(backend), _registreVariable(registreVariable), _registreType(registreType), _constructeurEquation(constructeurEquation)
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

    if (tokens[index].type == TOKEN_CROCHET_OUVERT) {
 
        consommer(tokens, index, TOKEN_CROCHET_OUVERT, "Erreur '['");
        expressionIndex = _constructeurEquation->construire(tokens, index);
        consommer(tokens, index, TOKEN_CROCHET_FERME, "Erreur : ']' attendu après l'index");
    }

    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    INoeud* expression = _constructeurEquation->construire(tokens, index);
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu");

    if (expressionIndex != nullptr) {
        return new NoeudAffectationTableau(nomVariable, expressionIndex, expression, nomToken);
    }
    
    return new NoeudAffectationVariable(nomVariable, expression, nomToken);
}