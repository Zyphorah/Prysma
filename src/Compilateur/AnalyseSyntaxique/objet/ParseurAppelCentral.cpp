#ifndef PARSEUR_APPELCENTRAL_CPP
#define PARSEUR_APPELCENTRAL_CPP

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Fonction/ParseurAppelFonction.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/objet/ParseurAppelObjet.h"

#include "Compilateur/objet/ParseurAppelCentral.h"

#include "Compilateur/AST/Registre/ContextParseur.h"
#include <cstddef>
#include <vector>

ParseurAppelCentral::ParseurAppelCentral(ContextParseur& contextParseur)
    : _contextParseur(contextParseur)
{}

ParseurAppelCentral::~ParseurAppelCentral()
= default;

INoeud* ParseurAppelCentral::parser(std::vector<Token>& tokens, int& index)
{
    const auto indexCourant = static_cast<size_t>(index);
    const bool appelObjet = indexCourant + 2 < tokens.size()
        && tokens[indexCourant].type == TOKEN_CALL
        && tokens[indexCourant + 1].type == TOKEN_IDENTIFIANT
        && tokens[indexCourant + 2].type == TOKEN_POINT;

    if (appelObjet) {
        ParseurAppelObjet parseurObjet(_contextParseur);
        return parseurObjet.parser(tokens, index);
    }

    ParseurAppelFonction parseurFonction(_contextParseur);
    return parseurFonction.parser(tokens, index);
}

#endif /* PARSEUR_APPELCENTRAL_CPP */