#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include <cstddef>
#include <llvm/Support/Allocator.h>
#include <vector>
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreExpression.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ChaineResponsabilite.h"
#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireParenthese.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"

ConstructeurArbreEquation::ConstructeurArbreEquation(
    ChaineResponsabilite* chaineResponsabilite,
    RegistreSymbole* registreSymbole,
    RegistreExpression* registreExpression,
    IGestionnaireParenthese* gestionnaireParenthese,
    llvm::BumpPtrAllocator& arena)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registreSymbole(registreSymbole), 
    _registreExpression(registreExpression),
      _gestionnaireParenthese(gestionnaireParenthese),
      _arena(arena),
      _dernierToken{TOKEN_EOF, "", 1, 1}
{
}

auto ConstructeurArbreEquation::construire(std::vector<Token> &equation) -> INoeud* {
    std::vector<Token> equationSansParentheses = _gestionnaireParenthese->enleverParenthesesEnglobantes(equation);
    equation = equationSansParentheses;
    
    if (equation.empty()) {
        throw ErreurCompilation("Erreur: équation vide", Ligne(_dernierToken.ligne), Colonne(_dernierToken.colonne));
    }
    
    int indice = _chaineResponsabilite->trouverOperateur(equation);

    if (indice == -1) {
        TokenType type = equation[0].type;

        if ((_registreExpression != nullptr) && _registreExpression->existe(type)) {
            return _registreExpression->recuperer(type)->construire(equation);
        }

        throw ErreurCompilation("Erreur: token non reconnu dans l'équation", Ligne(equation[0].ligne), Colonne(equation[0].colonne));
    }
    
    IExpression* noeud = _registreSymbole->recupererNoeud(equation[static_cast<size_t>(indice)]);
    std::vector<Token> gauche(equation.begin(), equation.begin() + indice); 
    std::vector<Token> droite(equation.begin() + indice + 1, equation.end());
    
    INoeud* exprGauche = construire(gauche);
    INoeud* exprDroite = construire(droite);
    
    noeud->ajouterExpression(exprGauche, exprDroite);
    return noeud;
}

auto ConstructeurArbreEquation::construire(std::vector<Token>& tokens, int& index) -> INoeud* {

    // Sauvegarder la position du token courant pour les messages d'erreur
    if (index < static_cast<int>(tokens.size())) {
        _dernierToken = tokens[static_cast<size_t>(index)];
    }

    // Système de niveau pour calculer la profondeur, c'est obligatoire pour ne pas avoir de problème au niveau de la séparation 34+4)) sinon le 
    // Système ne sais pas quoi faire avec les deux parenthèses restante. 
    
    std::vector<Token> equationTokens;
    int parenProfondeur = 0;
    int crochetProfondeur = 0;

    while(index < static_cast<int>(tokens.size())) {
        TokenType type = tokens[static_cast<size_t>(index)].type;

        if (type == TOKEN_PAREN_OUVERTE) {
            parenProfondeur++;
        }
        else if (type == TOKEN_PAREN_FERMEE) {
            if (parenProfondeur == 0) {
                break; 
            }
            parenProfondeur--;
        }
        else if (type == TOKEN_CROCHET_OUVERT) {
            crochetProfondeur++;
        }
        else if (type == TOKEN_CROCHET_FERME) {
            if (crochetProfondeur == 0) {
                break;
            }
            crochetProfondeur--;
        }
        
        if (parenProfondeur == 0 && crochetProfondeur == 0) {
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

llvm::BumpPtrAllocator& ConstructeurArbreEquation::getArena()
{
    return _arena;
}