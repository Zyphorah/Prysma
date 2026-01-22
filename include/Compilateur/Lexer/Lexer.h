#ifndef F2D02E97_AF62_409B_84AD_90905E9BE240
#define F2D02E97_AF62_409B_84AD_90905E9BE240
#include <map>
#include <string>
#include <vector>

using namespace std;
// lexer utilisée pour la tokenization du code source prysma
// filtre le code source en une liste de tokens identifier 
enum TokenType {
    // Fin de flux (Obligatoire)
    TOKEN_EOF,

    // Littéraux la valeur directe "string", 123, 12.34, true/false
    TOKEN_LIT_INT, 
    TOKEN_LIT_FLOAT,
    TOKEN_LIT_CHAINE,
    TOKEN_LIT_BOLEEN,

    TOKEN_IDENTIFIANT,  

    TOKEN_PLUS,         
    TOKEN_MOINS,        
    TOKEN_ETOILE,       
    TOKEN_SLASH,
    TOKEN_MODULO,
    TOKEN_PLUS_PETIT,
    TOKEN_PLUS_GRAND,
    TOKEN_PLUS_PETIT_EGAL,
    TOKEN_PLUS_GRAND_EGAL,      

    TOKEN_EGAL,         
    TOKEN_EGAL_EGAL,
    TOKEN_DIFFERENT,    

    // Structure
    TOKEN_PAREN_OUVERTE,
    TOKEN_PAREN_FERMEE,
    TOKEN_ACCOLADE_OUVERTE,
    TOKEN_ACCOLADE_FERMEE,
    TOKEN_CROCHET_OUVERT,
    TOKEN_CROCHET_FERME,
    TOKEN_POINT_VIRGULE,
    TOKEN_VIRGULE,      
    
    // Mots-clés (Mieux vaut être spécifique)
    TOKEN_FONCTION,    
    TOKEN_SI,          
    TOKEN_SINON,       
    TOKEN_TANT_QUE,
    TOKEN_POUR,
    TOKEN_RETOUR,
    TOKEN_VRAI,
    TOKEN_FAUX,
    TOKEN_PRINT,

    // variable 
    TOKEN_TYPE_INT,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_BOOL, 
    TOKEN_TYPE_VOID,
};

struct Token {
TokenType type;
string value;
};

class Lexer {
    private: 

    // Dictionnaire pour les mots réservé du langage de programmation prysma
    // Permet de différencier les identifiants des mots-clés
    map<string, TokenType> motsCles = {
        {"def", TOKEN_FONCTION},
        {"if", TOKEN_SI},
        {"else", TOKEN_SINON},
        {"while", TOKEN_TANT_QUE},
        {"for", TOKEN_POUR},
        {"return", TOKEN_RETOUR},
        {"true", TOKEN_VRAI},
        {"false", TOKEN_FAUX},
        {"print", TOKEN_PRINT},
        {"int", TOKEN_TYPE_INT},
        {"float", TOKEN_TYPE_FLOAT},
        {"string", TOKEN_TYPE_STRING},
        {"bool", TOKEN_TYPE_BOOL},
        {"void", TOKEN_TYPE_VOID},
        
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
