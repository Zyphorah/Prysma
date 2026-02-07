#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>
#include <memory>

ParseurEquation::ParseurEquation()
{
    _floatEquationBuilder = std::make_shared<FloatEquationBuilder>();
}

ParseurEquation::~ParseurEquation()
{
}

std::shared_ptr<INoeud> ParseurEquation::parser(std::vector<Token>& tokens, int& index,[[maybe_unused]] ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    size_t debutEquation = index;
    size_t finEquation = index;
    
    while (finEquation < tokens.size() && tokens[finEquation].type != TOKEN_POINT_VIRGULE) {
        finEquation++;
    }

    if (finEquation == tokens.size()) {
        throw std::runtime_error("Erreur : ';' manquant à la fin de la déclaration");
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
        throw std::runtime_error("Erreur : équation vide");
    }

    bool attendOperande = true;

    for (size_t i = 0; i < tokensEquation.size(); i++) {
        TokenType currentType = tokensEquation[i].type;

        if(currentType != TOKEN_PAREN_OUVERTE && currentType != TOKEN_PAREN_FERMEE)
        {
            if (attendOperande) {
                if (!estOperande(currentType)) {
                    throw std::runtime_error("Erreur : opérande attendu à la position " + std::to_string(i));
                }
                attendOperande = false;
            } else {
                if (!estOperateur(currentType)) {
                    throw std::runtime_error("Erreur : opérateur attendu à la position " + std::to_string(i));
                }
                attendOperande = true;
            }
        }
    }

    if (attendOperande) {
        throw std::runtime_error("Erreur : opérande attendu à la fin de l'équation");
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
