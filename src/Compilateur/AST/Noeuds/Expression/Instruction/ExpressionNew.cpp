#ifndef EXPRESSION_NEW_CPP
#define EXPRESSION_NEW_CPP

#include "Compilateur/Instruction/ExpressionNew.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Lexer/TokenType.h"

ExpressionNew::ExpressionNew(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionNew::~ExpressionNew()
{}

INoeud* ExpressionNew::construire(std::vector<Token>& equation)
{
    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_NEW) {
        throw ErreurCompilation("Erreur: le token 'new' est attendu", 0, 0);
    }
    index++;

    if (index >= static_cast<int>(equation.size())) {
        throw ErreurCompilation("Erreur: aucun type valide pour l'objet créé avec 'new'", equation[0].ligne, equation[0].colonne);
    }

    Token nomType = equation[static_cast<size_t>(index)];

    if (nomType.type != TOKEN_TYPE_INT32 &&
        nomType.type != TOKEN_TYPE_INT64 &&
        nomType.type != TOKEN_TYPE_FLOAT &&
        nomType.type != TOKEN_TYPE_BOOL &&
        nomType.type != TOKEN_TYPE_CHAR &&
        nomType.type != TOKEN_TYPE_STRING &&
        nomType.type != TOKEN_TYPE_VOID &&
        nomType.type != TOKEN_TYPE_PTR) {
        throw ErreurCompilation("Erreur: aucun type valide pour l'objet créé avec 'new'", nomType.ligne, nomType.colonne);
    }

    return new (_contexteExpression.arena.Allocate(sizeof(NoeudNew), alignof(NoeudNew))) NoeudNew(nomType);
}

#endif /* EXPRESSION_NEW_CPP */