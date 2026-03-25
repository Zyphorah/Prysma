#ifndef F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E
#define F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E

#include "Compiler/Lexer/TokenType.h"

constexpr auto estOperateur(TokenType type) noexcept -> bool {
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

constexpr auto estOperande(TokenType type) noexcept -> bool {
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

constexpr auto estOperateurComparison(TokenType type) noexcept -> bool {
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

constexpr auto estType(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_STRING:
        case TOKEN_TYPE_BOOL:
        case TOKEN_TYPE_VOID:
        case TOKEN_TYPE_CHAR:
        case TOKEN_TYPE_INT64:
        case TOKEN_TYPE_INT32:
        case TOKEN_TYPE_PTR:
        case TOKEN_IDENTIFIANT:
            return true;
        default:
            return false;
    }
}

constexpr auto estMotCle(TokenType type) noexcept -> bool {
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

constexpr auto estStructure(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_PAREN_OUVERTE:
        case TOKEN_PAREN_FERMEE:
        case TOKEN_ACCOLADE_OUVERTE:
        case TOKEN_ACCOLADE_FERMEE:
        case TOKEN_CROCHET_OUVERT:
        case TOKEN_CROCHET_FERME:
        case TOKEN_POINT:
        case TOKEN_POINT_VIRGULE:
        case TOKEN_VIRGULE:
            return true;
        default:
            return false;
    }
}

#endif /* F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E */
