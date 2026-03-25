#ifndef EXPRESSION_NEW_CPP
#define EXPRESSION_NEW_CPP

#include "Compiler/Instruction/ExpressionNew.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/ManagerError.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionNew::ExpressionNew(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionNew::~ExpressionNew()
= default;

auto ExpressionNew::construire(std::vector<Token>& equation) -> INode*
{
    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_NEW) {
        throw ErrorCompilation("Error: le token 'new' est attendu", Ligne(0), Colonne(0));
    }
    index++;

    if (index >= static_cast<int>(equation.size())) {
        throw ErrorCompilation("Error: aucun type valide pour l'object créé avec 'new'", Ligne(equation[0].ligne), Colonne(equation[0].colonne));
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
        throw ErrorCompilation("Error: aucun type valide pour l'object créé avec 'new'", Ligne(nomType.ligne), Colonne(nomType.colonne));
    }

    

    // Nous devons remplir les arguments du new (ex: new MaClasse(arg1, arg2)) en ajoutant les childs du nodeNew
    std::vector<INode*> arguments;
    if (nomType.type == TOKEN_IDENTIFIANT) {
        index++; // Passer le nom du type
        
        if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_PAREN_OUVERTE) {
            index++; // Passer '('

            while(index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type != TOKEN_PAREN_FERMEE) {
                
                arguments.push_back(_contexteExpression.getContextParser()->getBuilderTreeEquation()->construire(equation, index));

                if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_VIRGULE) {
                    index++; // Passer la virgule
                }
            }
            if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_PAREN_FERMEE) {
                index++; // Passer ')'
            }
        }
    }

    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeNew>(arguments, nomType);
}

#endif /* EXPRESSION_NEW_CPP */