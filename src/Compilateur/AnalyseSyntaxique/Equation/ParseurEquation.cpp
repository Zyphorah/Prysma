#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/GestionnaireErreur.h"
#include <stdexcept>
#include <memory>

ParseurEquation::ParseurEquation()
{
    _floatEquationBuilder = std::make_shared<ConstructeurEquationFlottante>();
}

ParseurEquation::~ParseurEquation()
{
}

std::shared_ptr<INoeud> ParseurEquation::parser(std::vector<Token>& tokens, int& index,[[maybe_unused]] IConstructeurArbre* constructeurArbre)
{
    size_t debutEquation = index;
    size_t finEquation = index;
    
    while (finEquation < tokens.size() && tokens[finEquation].type != TOKEN_POINT_VIRGULE) {
        finEquation++;
    }

    if (finEquation == tokens.size()) {
        throw ErreurCompilation("Erreur : ';' manquant à la fin de la déclaration", tokens.back().ligne, tokens.back().colonne);
    }

    std::vector<Token> tokensEquation(tokens.begin() + debutEquation, tokens.begin() + finEquation);
    
    validerEquation(tokensEquation);
    
    index = finEquation;
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu");

    return _floatEquationBuilder->construire(tokensEquation);
}

void ParseurEquation::validerEquation(const std::vector<Token>& tokensEquation) const
{
    if (tokensEquation.empty()) {
        throw ErreurCompilation("Erreur : équation vide", 1, 1);
    }

    bool attendOperande = true;

    for (size_t i = 0; i < tokensEquation.size(); i++) {
        TokenType currentType = tokensEquation[i].type;

        // Ignorer les parenthèses
        if(currentType == TOKEN_PAREN_OUVERTE || currentType == TOKEN_PAREN_FERMEE) {
            continue;
        }

        // Gérer les opérateurs unaires préfixe (unref, ref)
        if (estOperateurUnaire(currentType)) {
            if (!attendOperande) {
                throw ErreurCompilation("Erreur : opérateur unaire en mauvaise position à la position " + std::to_string(i), tokensEquation[i].ligne, tokensEquation[i].colonne);
            }
            continue;
        }

        if (attendOperande) {
            if (!estOperande(currentType)) {
                throw ErreurCompilation("Erreur : opérande attendu à la position " + std::to_string(i), tokensEquation[i].ligne, tokensEquation[i].colonne);
            }
            attendOperande = false;
        } else {
            if (!estOperateur(currentType)) {
                throw ErreurCompilation("Erreur : opérateur attendu à la position " + std::to_string(i), tokensEquation[i].ligne, tokensEquation[i].colonne);
            }
            attendOperande = true;
        }
    }

    if (attendOperande) {
        throw ErreurCompilation("Erreur : opérande attendu à la fin de l'équation", tokensEquation.back().ligne, tokensEquation.back().colonne);
    }
}

bool ParseurEquation::estOperateur(TokenType tokenType) const
{
    return TokenCategories::OPERATEURS.count(tokenType) > 0;
}

bool ParseurEquation::estOperande(TokenType tokenType) const
{
    return TokenCategories::OPERANDES.count(tokenType) > 0;
}

bool ParseurEquation::estOperateurUnaire(TokenType tokenType) const
{
    return tokenType == TOKEN_UNREF || tokenType == TOKEN_REF;
}
