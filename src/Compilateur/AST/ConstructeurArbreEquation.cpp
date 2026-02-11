#include "Compilateur/AST/ConstructeurArbreEquation.h"

#include <exception>
#include <utility>
#include <vector>
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudRefVariable.h"

ConstructeurArbreEquation::ConstructeurArbreEquation(
    ChaineResponsabilite* chaineResponsabilite,
    std::shared_ptr<RegistreSymbole> registreSymbole,
    IGestionnaireParenthese* gestionnaireParenthese,
    IConstructeurArbre* instructionBuilder)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registreSymbole(std::move(registreSymbole)), 
      _gestionnaireParenthese(gestionnaireParenthese),
      _instructionBuilder(instructionBuilder)
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

        if (equation[0].type == TOKEN_CALL) {
            int indexZero = 0;
            return _instructionBuilder->construire(equation, indexZero);
        }

        // Déterminer si c'est une référence ou déréférence
        if (equation[0].type == TOKEN_REF)
        {
            // Créer un noeud ref avec la variable suivante
            if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
                throw std::runtime_error("Erreur: 'ref' doit être suivi d'un identifiant");
            }
            return std::make_shared<NoeudRefVariable>(equation[1].value);
        }
        
        if (equation[0].type == TOKEN_UNREF)
        {
            // Créer un noeud unref avec la variable suivante
            if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
                throw std::runtime_error("Erreur: 'unref' doit être suivi d'un identifiant");
            }
            return std::make_shared<NoeudUnRefVariable>(equation[1].value);
        }
        
        if (equation[0].type == TOKEN_IDENTIFIANT && equation.size() == 1) {
     
            return std::make_shared<NoeudUnRefVariable>(equation[0].value);
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

std::shared_ptr<INoeud> ConstructeurArbreEquation::construire(std::vector<Token>& tokens, int& index) {
    std::vector<Token> equationTokens;
    while(index < (int)tokens.size() && 
          tokens[index].type != TOKEN_POINT_VIRGULE &&
          tokens[index].type != TOKEN_ACCOLADE_FERMEE &&
          tokens[index].type != TOKEN_PAREN_FERMEE &&
          tokens[index].type != TOKEN_VIRGULE) {
        equationTokens.push_back(tokens[index]);
        index++;
    }
    return construire(equationTokens);
}
