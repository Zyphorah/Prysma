#ifndef EXPRESSION_STRING_CPP
#define EXPRESSION_STRING_CPP

#include "Compiler/Math/ExpressionString.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

ExpressionString::ExpressionString(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionString::~ExpressionString()
= default;

auto ExpressionString::construire(std::vector<Token>& equation) -> INode*
{
    std::vector<INode*> elementsString;

    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_GUILLEMET) {
        throw std::runtime_error("Error: une chaine de caractere doit commencer par un guillemet");
    }
    index++;

    if (index >= static_cast<int>(equation.size()) || equation[static_cast<size_t>(index)].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Error: une chaine de caractere doit etre composee de caracteres alphanumeriques");
    }
    Token chaine = equation[static_cast<size_t>(index)];
    index++;

    if (index >= static_cast<int>(equation.size()) || equation[static_cast<size_t>(index)].type != TOKEN_GUILLEMET) {
        throw std::runtime_error("Error: une chaine de caractere doit finir par un guillemet");
    }

    for (size_t caractereIndex = 0; caractereIndex < chaine.value.size(); caractereIndex++) {
        int ascii = static_cast<unsigned char>(chaine.value[caractereIndex]);
        Token token;
        token.type = TOKEN_LIT_INT;
        token.value = std::to_string(ascii);
        token.ligne = chaine.ligne;
        token.colonne = chaine.colonne;
        elementsString.push_back(_contexteExpression.getBuilderTreeEquation()->allouer<NodeLiteral>(token)); 
    }

    Token tokenZero;
    tokenZero.type = TOKEN_LIT_INT;
    tokenZero.value = "0";
    tokenZero.ligne = chaine.ligne;
    tokenZero.colonne = chaine.colonne;
    elementsString.push_back(_contexteExpression.getBuilderTreeEquation()->allouer<NodeLiteral>(tokenZero)); 

    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeArrayInitialization>(elementsString);
}

#endif /* EXPRESSION_STRING_CPP */