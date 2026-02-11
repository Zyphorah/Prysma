#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/GestionnaireErreur.h"
#include <memory>

ParseurDeclarationVariable::ParseurDeclarationVariable()
{}

ParseurDeclarationVariable::~ParseurDeclarationVariable()
{
}

std::shared_ptr<INoeud> ParseurDeclarationVariable::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    consommer(tokens,index,TOKEN_DEC,"Erreur : type attendu 'dec");
    
    // Vérifier que le token courant est un type valide
    if (TokenCategories::TYPES.find(tokens[index].type) == TokenCategories::TYPES.end()) {
        throw ErreurCompilation("Erreur : type attendu après 'dec'", tokens[index].ligne, tokens[index].colonne);
    }
    Token typeToken = tokens[index];
    index++;
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    ParseurEquation parseurEquation{};
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbre);

    return std::make_shared<NoeudDeclarationVariable>(
        nomVariable,
        expression,
        typeToken.type
    );
}
