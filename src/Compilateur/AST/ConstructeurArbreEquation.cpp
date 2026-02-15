#include "Compilateur/AST/ConstructeurArbreEquation.h"

#include <exception>
#include <utility>
#include <vector>
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
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
  
            ParseurAppelFonction parseurAppel;
            int indexZero = 0;
            std::vector<Token> equationAvecIndex = equation;
            return parseurAppel.parser(equationAvecIndex, indexZero, _instructionBuilder);
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
        
        if (equation[0].type == TOKEN_NON)
        {
            // Créer un noeud de négation avec l'expression suivante
            if (equation.size() < 2) {
                throw std::runtime_error("Erreur: '!' doit être suivi d'une expression");
            }
            std::vector<Token> operande(equation.begin() + 1, equation.end());
            std::shared_ptr<INoeud> exprOperande = construire(operande);
            return std::make_shared<NoeudNegation>(equation[0], exprOperande);
        }
        
        if (equation[0].type == TOKEN_LIT_INT || equation[0].type == TOKEN_LIT_FLOAT || equation[0].type == TOKEN_LIT_BOLEEN) {
            return std::make_shared<NoeudLitteral>(equation[0]);
        }

        if (equation[0].type == TOKEN_IDENTIFIANT && equation.size() == 1) {
     
            return std::make_shared<NoeudUnRefVariable>(equation[0].value); 
        }
        

        return std::make_shared<NoeudUnRefVariable>(equation[0].value);
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
    
    if (index < (int)tokens.size() && tokens[index].type == TOKEN_CALL && _instructionBuilder != nullptr) {
        // Déléguer au parseur d'expression directement
        ParseurAppelFonction parseurAppel;
        return parseurAppel.parser(tokens, index, _instructionBuilder);
    }
    
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