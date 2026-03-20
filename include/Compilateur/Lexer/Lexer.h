#ifndef F2D02E97_AF62_409B_84AD_90905E9BE240
#define F2D02E97_AF62_409B_84AD_90905E9BE240
#include "TokenType.h"
#include <array>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

using namespace std;
// lexer utilisé pour la tokenization du code source prysma
// filtre le code source en une liste de tokens identifiés 

struct Token {
    TokenType type;
    string value;
    int ligne;      
    int colonne;    
};

class Lexer {
    private: 

    // Dictionnaire pour les mots réservés du langage de programmation prysma
    // Permet de différencier les identifiants des mots-clés
    static constexpr std::array<std::pair<const char*, TokenType>, 31> motsClesArray = {{
        {"char", TOKEN_TYPE_CHAR},
        {"arg", TOKEN_ARG},
        {"fn", TOKEN_FONCTION},
        {"if", TOKEN_SI},
        {"else", TOKEN_SINON},
        {"while", TOKEN_TANT_QUE},
        {"for", TOKEN_POUR},
        {"return", TOKEN_RETOUR},
        {"true", TOKEN_VRAI},
        {"false", TOKEN_FAUX},
        {"scope", TOKEN_SCOPE},
        {"aff", TOKEN_AFF},
        {"dec", TOKEN_DEC},
        {"int64", TOKEN_TYPE_INT64},
        {"int32", TOKEN_TYPE_INT32},
        {"float", TOKEN_TYPE_FLOAT},
        {"string", TOKEN_TYPE_STRING},
        {"bool", TOKEN_TYPE_BOOL},
        {"void", TOKEN_TYPE_VOID},
        {"call", TOKEN_CALL},
        {"pass", TOKEN_PASS},
        {"ref", TOKEN_REF},
        {"unref", TOKEN_UNREF},
        {"include", TOKEN_INCLUDE},
        {"new", TOKEN_NEW},
        {"ptr",TOKEN_TYPE_PTR},
        {"delete", TOKEN_DELETE},
        {"class", TOKEN_CLASS},
        {"public", TOKEN_PUBLIC},
        {"private", TOKEN_PRIVATE},
        {"protected",TOKEN_PROTECTED},
    }};

    // Fonctions privées pour la tokenization
    static void ajouterMotCourant(const string& motCourant, vector<Token>& tokens, int ligne, int colonne);
    static void traiterOperateursEtDelimiteurs(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne);
    static void traiterOperateursMathematiques(char current, vector<Token>& tokens, int ligne, int colonne);
    static void traiterDelimiteurs(char current, vector<Token>& tokens, int ligne, int colonne);
    static void traiterOperateursComplexes(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne);
    static void traiterLitteraux(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne);
    static void traiterCommentaires(const string& sourceCode, size_t& pos);
    static void traiterNombre(const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne);
    static auto estContexteNombreNegatif(const vector<Token>& tokens) -> bool;
    static auto traiterNombreNegatifOuPositif(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, 
                                        string& motCourant, int ligne, int& colonne, int& colonneMotCourant) -> bool;
    
    public: 
    static auto tokenizer(const string& sourceCode) -> vector<Token>;
};

#endif /* F2D02E97_AF62_409B_84AD_90905E9BE240 */
