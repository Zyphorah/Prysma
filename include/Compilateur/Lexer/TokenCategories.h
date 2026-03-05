#ifndef F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E
#define F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E

#include "Compilateur/Lexer/TokenType.h"

constexpr bool estOperateur(TokenType type) noexcept {
    switch (type) {
        case TOKEN_PLUS:
        case TOKEN_MOINS:
        case TOKEN_ETOILE:
        case TOKEN_SLASH:
        case TOKEN_MODULO:
            return true;
        default:
            return false;
    }
}

constexpr bool estOperande(TokenType type) noexcept {
    switch (type) {
        case TOKEN_LIT_INT:
        case TOKEN_LIT_FLOAT:
        case TOKEN_LIT_BOLEEN:
        case TOKEN_IDENTIFIANT:
        case TOKEN_UNREF:
        case TOKEN_REF:
            return true;
        default:
            return false;
    }
}

constexpr bool estOperateurComparaison(TokenType type) noexcept {
    switch (type) {
        case TOKEN_PLUS_PETIT:
        case TOKEN_PLUS_GRAND:
        case TOKEN_PLUS_PETIT_EGAL:
        case TOKEN_PLUS_GRAND_EGAL:
        case TOKEN_EGAL_EGAL:
        case TOKEN_DIFFERENT:
            return true;
        default:
            return false;
    }
}

constexpr bool estType(TokenType type) noexcept {
    switch (type) {
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_STRING:
        case TOKEN_TYPE_BOOL:
        case TOKEN_TYPE_VOID:
        case TOKEN_TYPE_CHAR:
        case TOKEN_TYPE_INT64:
        case TOKEN_TYPE_INT32:
            return true;
        default:
            return false;
    }
}

constexpr bool estMotCle(TokenType type) noexcept {
    switch (type) {
        case TOKEN_FONCTION:
        case TOKEN_SI:
        case TOKEN_SINON:
        case TOKEN_TANT_QUE:
        case TOKEN_POUR:
        case TOKEN_RETOUR:
        case TOKEN_SCOPE:
        case TOKEN_AFF:
        case TOKEN_DEC:
            return true;
        default:
            return false;
    }
}

constexpr bool estStructure(TokenType type) noexcept {
    switch (type) {
        case TOKEN_PAREN_OUVERTE:
        case TOKEN_PAREN_FERMEE:
        case TOKEN_ACCOLADE_OUVERTE:
        case TOKEN_ACCOLADE_FERMEE:
        case TOKEN_CROCHET_OUVERT:
        case TOKEN_CROCHET_FERME:
        case TOKEN_POINT_VIRGULE:
        case TOKEN_VIRGULE:
            return true;
        default:
            return false;
    }
}

#endif /* F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E */
