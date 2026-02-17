#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <memory>
#include <utility>

ParseurAffectationVariable::ParseurAffectationVariable(std::shared_ptr<LlvmBackend> backend, std::shared_ptr<RegistreVariable> registreVariable,std::shared_ptr<RegistreType> registreType, IConstructeurArbre* constructeurEquation)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _registreType(std::move(registreType)), _constructeurEquation(constructeurEquation)
{
}

ParseurAffectationVariable::~ParseurAffectationVariable()
{
}

std::shared_ptr<INoeud> ParseurAffectationVariable::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    consommer(tokens, index, TOKEN_AFF, "Erreur : l'affectation est attendu 'aff'");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    
    std::shared_ptr<INoeud> expression;
    if (_constructeurEquation != nullptr) {
        expression = _constructeurEquation->construire(tokens, index);
        consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'affectation");
    } else {
        ParseurEquation parseurEquation(constructeurArbre);
        expression = parseurEquation.parser(tokens, index, constructeurArbre);
    }

    return std::make_shared<NoeudAffectationVariable>(nomVariable, expression, nomToken);
}
