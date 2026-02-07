#include "Compilateur/AST/ConstructeurArbreEquation.h"

#include <utility>
#include <vector>
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"

ConstructeurArbreEquation::ConstructeurArbreEquation(
    ChaineResponsabilite* chaineResponsabilite,
    std::shared_ptr<RegistreSymbole> registreSymbole,
    IGestionnaireParenthese* gestionnaireParenthese)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registreSymbole(std::move(registreSymbole)), 
      _gestionnaireParenthese(gestionnaireParenthese)
{
}

// "Pile d'Appels" (Call Stack) 
std::shared_ptr<INoeud> ConstructeurArbreEquation::construire(std::vector<Token> &equation) {
    // Enlever les parenthèses englobantes
    std::vector<Token> equationSansParentheses = _gestionnaireParenthese->enleverParenthesesEnglobantes(equation);
    equation = equationSansParentheses;
    
    if (equation.empty()) {
        throw std::runtime_error("Erreur: équation vide");
    }
    
    int indice = _chaineResponsabilite->trouverOperateur(equation);
    
    if (indice == -1) {
        // Déterminer si c'est une variable
        if(equation[0].type == TOKEN_IDENTIFIANT)
        {
            return std::make_shared<NoeudVariable>(equation[0].value);
        }
        try {
            float valeurFloat = std::stof(equation[0].value);
            return std::make_shared<NoeudLitteral>(valeurFloat);
        } catch (const std::exception& e) {
            throw std::runtime_error("Erreur: impossible de convertir '" + equation[0].value + "' en nombre");
        }    
    }
    
    std::shared_ptr<IExpression> noeud = _registreSymbole->recupererNoeud(equation[indice].type);
    std::vector<Token> gauche(equation.begin(), equation.begin() + indice);
    std::vector<Token> droite(equation.begin() + indice + 1, equation.end());
    
    std::shared_ptr<INoeud> exprGauche = construire(gauche);
    std::shared_ptr<INoeud> exprDroite = construire(droite);
    
    noeud->ajouterExpression(exprGauche, exprDroite);
    return noeud;
}
