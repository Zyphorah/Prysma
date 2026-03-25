#include "Compiler/AST/BuilderTreeEquation.h"
#include <cstddef>
#include <llvm/Support/Allocator.h>
#include <vector>
#include "Compiler/AST/Nodes/Interfaces/IExpression.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/RegistryExpression.h"
#include "Compiler/AST/Registry/RegistrySymbole.h"
#include "Compiler/Parser/Equation/ChainOfResponsibility.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/ManagerError.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"

BuilderTreeEquation::BuilderTreeEquation(
    ChainOfResponsibility* chaineResponsabilite,
    RegistrySymbole* registrySymbole,
    RegistryExpression* registryExpression,
    IManagerParenthese* managerParenthese,
    llvm::BumpPtrAllocator& arena)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registrySymbole(registrySymbole), 
    _registryExpression(registryExpression),
      _managerParenthese(managerParenthese),
      _arena(arena),
      _dernierToken{TOKEN_EOF, "", 1, 1}
{
}

auto BuilderTreeEquation::construire(std::vector<Token> &equation) -> INode* {
    std::vector<Token> equationSansParentheses = _managerParenthese->enleverParenthesesEnglobantes(equation);
    equation = equationSansParentheses;
    
    if (equation.empty()) {
        throw ErrorCompilation("Error: équation vide", Ligne(_dernierToken.ligne), Colonne(_dernierToken.colonne));
    }
    
    int indice = _chaineResponsabilite->trouverOperateur(equation);

    if (indice == -1) {
        TokenType type = equation[0].type;

        if ((_registryExpression != nullptr) && _registryExpression->existe(type)) {
            return _registryExpression->recuperer(type)->construire(equation);
        }

        throw ErrorCompilation("Error: token non reconnu dans l'équation", Ligne(equation[0].ligne), Colonne(equation[0].colonne));
    }
    
    IExpression* node = _registrySymbole->recupererNode(equation[static_cast<size_t>(indice)]);
    std::vector<Token> gauche(equation.begin(), equation.begin() + indice); 
    std::vector<Token> droite(equation.begin() + indice + 1, equation.end());
    
    INode* exprGauche = construire(gauche);
    INode* exprDroite = construire(droite);
    
    node->ajouterExpression(exprGauche, exprDroite);
    return node;
}

auto BuilderTreeEquation::construire(std::vector<Token>& tokens, int& index) -> INode* {

    // Sauvegarder la position du token courant pour les messages d'error
    if (index < static_cast<int>(tokens.size())) {
        _dernierToken = tokens[static_cast<size_t>(index)];
    }

    // Système de niveau pour calculer la depth, c'est obligatoire pour ne pas avoir de problème au niveau de la séparation 34+4)) sinon le 
    // Système ne sais pas quoi faire avec les deux parenthèses restante. 
    
    std::vector<Token> equationTokens;
    int parenDepth = 0;
    int crochetDepth = 0;

    while(index < static_cast<int>(tokens.size())) {
        TokenType type = tokens[static_cast<size_t>(index)].type;

        if (type == TOKEN_PAREN_OUVERTE) {
            parenDepth++;
        }
        else if (type == TOKEN_PAREN_FERMEE) {
            if (parenDepth == 0) {
                break; 
            }
            parenDepth--;
        }
        else if (type == TOKEN_CROCHET_OUVERT) {
            crochetDepth++;
        }
        else if (type == TOKEN_CROCHET_FERME) {
            if (crochetDepth == 0) {
                break;
            }
            crochetDepth--;
        }
        
        if (parenDepth == 0 && crochetDepth == 0) {
            if (type == TOKEN_POINT_VIRGULE || 
                type == TOKEN_ACCOLADE_FERMEE || 
                type == TOKEN_VIRGULE ) {
                break;
            }
        }

        equationTokens.push_back(tokens[static_cast<size_t>(index)]);
        index++;
    }

    return construire(equationTokens);
}

llvm::BumpPtrAllocator& BuilderTreeEquation::getArena()
{
    return _arena;
}