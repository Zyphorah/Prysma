#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include <memory>
#include <utility>

ParseurAffectationVariable::ParseurAffectationVariable(std::shared_ptr<LlvmBackend> backend, std::shared_ptr<RegistreVariable> registreVariable,std::shared_ptr<RegistreType> registreType, IConstructeurArbre* constructeurEquation)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _registreType(std::move(registreType)), _constructeurEquation(constructeurEquation)
{
}

ParseurAffectationVariable::~ParseurAffectationVariable()
{
}

std::shared_ptr<INoeud> ParseurAffectationVariable::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_AFF, "Erreur : l'affectation est attendu 'aff'");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    
    std::shared_ptr<INoeud> expression = _constructeurEquation->construire(tokens, index);
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'affectation");

    return std::make_shared<NoeudAffectationVariable>(nomVariable, expression, nomToken);
}
