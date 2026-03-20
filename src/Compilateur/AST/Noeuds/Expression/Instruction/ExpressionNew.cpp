#ifndef EXPRESSION_NEW_CPP
#define EXPRESSION_NEW_CPP

#include "Compilateur/Instruction/ExpressionNew.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionNew::ExpressionNew(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionNew::~ExpressionNew()
= default;

auto ExpressionNew::construire(std::vector<Token>& equation) -> INoeud*
{
    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_NEW) {
        throw ErreurCompilation("Erreur: le token 'new' est attendu", Ligne(0), Colonne(0));
    }
    index++;

    if (index >= static_cast<int>(equation.size())) {
        throw ErreurCompilation("Erreur: aucun type valide pour l'objet créé avec 'new'", Ligne(equation[0].ligne), Colonne(equation[0].colonne));
    }

    Token nomType = equation[static_cast<size_t>(index)];

    if (nomType.type != TOKEN_TYPE_INT32 &&
        nomType.type != TOKEN_TYPE_INT64 &&
        nomType.type != TOKEN_TYPE_FLOAT &&
        nomType.type != TOKEN_TYPE_BOOL &&
        nomType.type != TOKEN_TYPE_CHAR &&
        nomType.type != TOKEN_TYPE_STRING &&
        nomType.type != TOKEN_TYPE_VOID &&
        nomType.type != TOKEN_TYPE_PTR &&
        nomType.type != TOKEN_IDENTIFIANT) {
        throw ErreurCompilation("Erreur: aucun type valide pour l'objet créé avec 'new'", Ligne(nomType.ligne), Colonne(nomType.colonne));
    }

    

    // Nous devons remplir les arguments du new (ex: new MaClasse(arg1, arg2)) en ajoutant les enfants du noeudNew
    std::vector<INoeud*> arguments;
    if (nomType.type == TOKEN_IDENTIFIANT) {
        index++; // Passer le nom du type
        
        if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_PAREN_OUVERTE) {
            index++; // Passer '('

            while(index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type != TOKEN_PAREN_FERMEE) {
                
                arguments.push_back(_contexteExpression.getContextParseur()->getConstructeurArbreEquation()->construire(equation, index));

                if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_VIRGULE) {
                    index++; // Passer la virgule
                }
            }
            if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_PAREN_FERMEE) {
                index++; // Passer ')'
            }
        }
    }

    return new (_contexteExpression.getArena()->Allocate(sizeof(NoeudNew), alignof(NoeudNew))) NoeudNew(arguments,nomType); // NOLINT
}

#endif /* EXPRESSION_NEW_CPP */