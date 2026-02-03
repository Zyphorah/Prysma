#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclaration.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include <memory>
#include <iostream> 

ParseurDeclaration::ParseurDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::shared_ptr<RegistreType> registreType)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _registreType(std::move(registreType))
{
}

ParseurDeclaration::~ParseurDeclaration()
{
}

std::shared_ptr<INoeud> ParseurDeclaration::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    consommer(tokens,index,TOKEN_DEC,"Erreur : type attendu 'dec");
    
    // Vérifier que le token courant est un type valide
    if (TokenCategories::TYPES.find(tokens[index].type) == TokenCategories::TYPES.end()) {
        throw std::runtime_error("Erreur : type attendu après 'dec'");
    }
    Token typeToken = tokens[index];
    index++;
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    ParseurEquation parseurEquation(_backend, _registreVariable);
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    return std::make_shared<NoeudDeclaration>(
        _backend,
        _registreVariable,
        nomVariable,
        expression,
        _registreType,
        typeToken.type
    );
}
