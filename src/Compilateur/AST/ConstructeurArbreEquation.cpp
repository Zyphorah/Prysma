#include "Compilateur/AST/ConstructeurArbreEquation.h"

#include <exception>
#include <utility>
#include <vector>
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"

ConstructeurArbreEquation::ConstructeurArbreEquation(
    ChaineResponsabilite* chaineResponsabilite,
    std::shared_ptr<RegistreSymbole> registreSymbole,
    std::shared_ptr<RegistreStrategieEquation> registreStrategieEquation,
    IGestionnaireParenthese* gestionnaireParenthese,
    IConstructeurArbre* instructionBuilder)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registreSymbole(std::move(registreSymbole)), 
      _registreStrategieEquation(std::move(registreStrategieEquation)),
      _gestionnaireParenthese(gestionnaireParenthese),
      _instructionBuilder(instructionBuilder)
{
}

std::shared_ptr<INoeud> ConstructeurArbreEquation::construire(std::vector<Token> &equation) {
    std::vector<Token> equationSansParentheses = _gestionnaireParenthese->enleverParenthesesEnglobantes(equation);
    equation = equationSansParentheses;
    
    if (equation.empty()) {
        throw std::runtime_error("Erreur: équation vide");
    }
    
    int indice = _chaineResponsabilite->trouverOperateur(equation);

    if (indice == -1) {
        TokenType type = equation[0].type;

        if (_registreStrategieEquation->existe(type)) {
            return _registreStrategieEquation->recuperer(type)->construire(equation, this, _instructionBuilder);
        }

        throw std::runtime_error("Erreur: token non reconnu dans l'équation");
    }
    
    std::shared_ptr<IExpression> noeud = _registreSymbole->recupererNoeud(equation[indice]);
    std::vector<Token> gauche(equation.begin(), equation.begin() + indice); 
    std::vector<Token> droite(equation.begin() + indice + 1, equation.end());
    
    std::shared_ptr<INoeud> exprGauche = construire(gauche);
    std::shared_ptr<INoeud> exprDroite = construire(droite);
    
    noeud->ajouterExpression(exprGauche, exprDroite);
    return noeud;
}

std::shared_ptr<INoeud> ConstructeurArbreEquation::construire(std::vector<Token>& tokens, int& index) {

    // Système de niveau pour calculer la profondeur, c'est obligatoire pour ne pas avoir de problème au niveau de la séparation 34+4)) sinon le 
    // Système ne sais pas quoi faire avec les deux parenthèses restante. 
    
    std::vector<Token> equationTokens;
    int parenProfondeur = 0;

    while(index < (int)tokens.size()) {
        TokenType type = tokens[index].type;

        if (type == TOKEN_PAREN_OUVERTE) {
            parenProfondeur++;
        }
        else if (type == TOKEN_PAREN_FERMEE) {
            if (parenProfondeur == 0) {
                break; 
            }
            parenProfondeur--;
        }
        
        if (parenProfondeur == 0) {
            if (type == TOKEN_POINT_VIRGULE || 
                type == TOKEN_ACCOLADE_FERMEE || 
                type == TOKEN_VIRGULE) {
                break;
            }
        }

        equationTokens.push_back(tokens[index]);
        index++;
    }

    return construire(equationTokens);
}