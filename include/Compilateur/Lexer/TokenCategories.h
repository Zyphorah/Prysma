#ifndef F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E
#define F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E

#include "Compilateur/Lexer/TokenType.h"
#include <unordered_set>

namespace TokenCategories {
    inline const std::unordered_set<TokenType> OPERATEURS = {
        TOKEN_PLUS,
        TOKEN_MOINS,
        TOKEN_ETOILE,
        TOKEN_SLASH,
        TOKEN_MODULO
    };

    inline const std::unordered_set<TokenType> OPERANDES = {
        TOKEN_LIT_INT,
        TOKEN_LIT_FLOAT,
        TOKEN_LIT_BOLEEN,
        TOKEN_IDENTIFIANT,
        TOKEN_UNREF,
        TOKEN_REF,
    };

    inline const std::unordered_set<TokenType> OPERATEURS_COMPARAISON = {
        TOKEN_PLUS_PETIT,
        TOKEN_PLUS_GRAND,
        TOKEN_PLUS_PETIT_EGAL,
        TOKEN_PLUS_GRAND_EGAL,
        TOKEN_EGAL_EGAL,
        TOKEN_DIFFERENT
    };

    inline const std::unordered_set<TokenType> TYPES = {
        TOKEN_TYPE_FLOAT,
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_BOOL,
        TOKEN_TYPE_VOID,
        TOKEN_TYPE_CHAR,
         TOKEN_TYPE_INT64,
        TOKEN_TYPE_INT32
    };

    inline const std::unordered_set<TokenType> MOTS_CLES = {
        TOKEN_FONCTION,
        TOKEN_SI,
        TOKEN_SINON,
        TOKEN_TANT_QUE,
        TOKEN_POUR,
        TOKEN_RETOUR,
        TOKEN_PRINT,
        TOKEN_SCOPE,
        TOKEN_AFF,
        TOKEN_DEC
    };

    inline const std::unordered_set<TokenType> STRUCTURE = {
        TOKEN_PAREN_OUVERTE,
        TOKEN_PAREN_FERMEE,
        TOKEN_ACCOLADE_OUVERTE,
        TOKEN_ACCOLADE_FERMEE,
        TOKEN_CROCHET_OUVERT,
        TOKEN_CROCHET_FERME,
        TOKEN_POINT_VIRGULE,
        TOKEN_VIRGULE
    };
}

#endif /* F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E */
