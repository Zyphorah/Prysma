#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include <memory>
#include <utility>

ParseurDeclarationVariable::ParseurDeclarationVariable(std::shared_ptr<ParseurType> parseurType, IConstructeurArbre* constructeurEquation)
    : _parseurType(std::move(parseurType)), _constructeurEquation(constructeurEquation)
{}

ParseurDeclarationVariable::~ParseurDeclarationVariable()
{
}

std::shared_ptr<INoeud> ParseurDeclarationVariable::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    consommer(tokens, index, TOKEN_DEC, "Erreur : type attendu 'dec");
    
    // Utiliser le ParseurType pour analyser le type (simple ou tableau)
    std::shared_ptr<IType> type = _parseurType->parser(tokens, index);
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    // Use the equation builder if available, otherwise try to extract and parse the equation
    std::shared_ptr<INoeud> expression;
    if (_constructeurEquation != nullptr) {
        expression = _constructeurEquation->construire(tokens, index);
    } else {
        ParseurEquation parseurEquation(constructeurArbre);
        expression = parseurEquation.parser(tokens, index, constructeurArbre);
        return std::make_shared<NoeudDeclarationVariable>(nomVariable, type, expression);
    }
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de la déclaration");

    return std::make_shared<NoeudDeclarationVariable>(
        nomVariable,
        type,
        expression
    );
}
