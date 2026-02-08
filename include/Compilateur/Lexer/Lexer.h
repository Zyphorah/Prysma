#ifndef F2D02E97_AF62_409B_84AD_90905E9BE240
#define F2D02E97_AF62_409B_84AD_90905E9BE240
#include <map>
#include "TokenType.h"
#include <string>
#include <vector>

using namespace std;
// lexer utilisé pour la tokenization du code source prysma
// filtre le code source en une liste de tokens identifiés 

struct Token {
TokenType type;
string value;
};

class Lexer {
    private: 

    // Dictionnaire pour les mots réservés du langage de programmation prysma
    // Permet de différencier les identifiants des mots-clés
    map<string, TokenType> motsCles = {
        {"arg", TOKEN_ARG},
        {"fn", TOKEN_FONCTION},
        {"if", TOKEN_SI},
        {"else", TOKEN_SINON},
        {"while", TOKEN_TANT_QUE},
        {"for", TOKEN_POUR},
        {"return", TOKEN_RETOUR},
        {"true", TOKEN_VRAI},
        {"false", TOKEN_FAUX},
        {"print", TOKEN_PRINT},
        {"scope", TOKEN_SCOPE},
        {"aff", TOKEN_AFF},
        {"dec", TOKEN_DEC},
        {"int", TOKEN_TYPE_INT},
        {"float", TOKEN_TYPE_FLOAT},
        {"string", TOKEN_TYPE_STRING},
        {"bool", TOKEN_TYPE_BOOL},
        {"void", TOKEN_TYPE_VOID},
        {"call", TOKEN_CALL},
        {"pass", TOKEN_PASS},
        {"ref", TOKEN_REF},
        {"unref", TOKEN_UNREF}
        
    };

    // Fonctions privées pour la tokenization
    void ajouterMotCourant(const string& motCourant, vector<Token>& tokens);
    void traiterOperateursEtDelimiteurs(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens);
    void traiterOperateursMathematiques(char current, vector<Token>& tokens);
    void traiterDelimiteurs(char current, vector<Token>& tokens);
    void traiterOperateursComplexes(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens);
    void traiterLitteraux(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens);
    void traiterCommentaires(const string& sourceCode, size_t& pos);
    void traiterChaine(const string& sourceCode, size_t& pos, vector<Token>& tokens);
    void traiterNombre(const string& sourceCode, size_t& pos, vector<Token>& tokens);
    
    public: 
        Lexer() {}
        ~Lexer() {}

        vector<Token> tokenizer(const string& sourceCode);
};

#endif /* F2D02E97_AF62_409B_84AD_90905E9BE240 */
