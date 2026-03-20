#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cctype>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

auto Lexer::estContexteNombreNegatif(const vector<Token>& tokens) -> bool {
    if (tokens.empty()) {
        return true;
    }
    
    TokenType lastType = tokens.back().type;
    
    // Les contextes où +/- sont des opérateurs unaires
    static const set<TokenType> operateurUnaire = {
        TOKEN_EGAL, TOKEN_PAREN_OUVERTE, TOKEN_VIRGULE, TOKEN_CROCHET_OUVERT,
        TOKEN_PLUS, TOKEN_MOINS, TOKEN_ETOILE, TOKEN_SLASH, TOKEN_MODULO,
        TOKEN_EGAL_EGAL, TOKEN_DIFFERENT, TOKEN_PLUS_PETIT, TOKEN_PLUS_GRAND,
        TOKEN_PLUS_PETIT_EGAL, TOKEN_PLUS_GRAND_EGAL, TOKEN_ET, TOKEN_OU
    };
    
    return operateurUnaire.count(lastType) > 0;
}

void Lexer::traiterCommentaires(const string& sourceCode, size_t& pos) {
    if (pos + 1 < sourceCode.length() && sourceCode[pos] == '/' && sourceCode[pos + 1] == '/') {
        while (pos < sourceCode.length() && sourceCode[pos] != '\n') {
            pos++;
        }
        return;
    }
    
    if (pos + 1 < sourceCode.length() && sourceCode[pos] == '/' && sourceCode[pos + 1] == '*') {
        pos += 2;
        while (pos + 1 < sourceCode.length()) {
            if (sourceCode[pos] == '*' && sourceCode[pos + 1] == '/') {
                pos += 2;
                return;
            }
            pos++;
        }
        pos++;
    }
}

void Lexer::traiterNombre(const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne) {
    int colonneDebut = colonne;
    string nombre;
    bool estFlottant = false;
    
    if ((sourceCode[pos] == '-' || sourceCode[pos] == '+') && 
        pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0) {
        nombre += sourceCode[pos];
        pos++;
        colonne++;
    }
    
    while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
        nombre += sourceCode[pos];
        pos++;
        colonne++;
    }
    
    if (pos < sourceCode.length() && sourceCode[pos] == '.') {
        estFlottant = true;
        nombre += sourceCode[pos];
        pos++;
        colonne++;
        
        while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
            nombre += sourceCode[pos];
            pos++;
            colonne++;
        }
    }
    
    if (estFlottant) {
        tokens.push_back({TOKEN_LIT_FLOAT, nombre, ligne, colonneDebut});
    } else {
        tokens.push_back({TOKEN_LIT_INT, nombre, ligne, colonneDebut});
    }
}

void Lexer::traiterLitteraux(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne) {
    if (isdigit(current) != 0) {
        traiterNombre(sourceCode, pos, tokens, ligne, colonne);
    }
}

void Lexer::ajouterMotCourant(const string& motCourant, vector<Token>& tokens
    , int ligne 
    , int colonne 
    )
{

    if (motCourant.empty()) {
        return;
    }

    TokenType type = TOKEN_IDENTIFIANT;
    for (const auto& [keyword, tokenType] : motsClesArray) {
        if (keyword == motCourant) {
            type = tokenType;
            break;
        }
    }
    Token token;
    
    // Convertir les booléens true/false en littéraux numériques
    if (type == TOKEN_VRAI) {
        token.type = TOKEN_LIT_BOLEEN;
        token.value = "1";
    } else if (type == TOKEN_FAUX) {
        token.type = TOKEN_LIT_BOLEEN;
        token.value = "0";
    } else {
        token.type = type;
        token.value = motCourant;
    }
    
    token.ligne = ligne;
    token.colonne = colonne;
    
    tokens.push_back(token);
}

void Lexer::traiterOperateursMathematiques(char current, vector<Token>& tokens, int ligne, int colonne) {
    switch (current) {
        case '+': 
            tokens.push_back({TOKEN_PLUS, "+", ligne, colonne}); 
            break;
        case '-': 
            tokens.push_back({TOKEN_MOINS, "-", ligne, colonne}); 
            break;
        case '*': 
            tokens.push_back({TOKEN_ETOILE, "*", ligne, colonne}); 
            break;
        case '/': 
            tokens.push_back({TOKEN_SLASH, "/", ligne, colonne}); 
            break;
        case '%':
            tokens.push_back({TOKEN_MODULO, "%", ligne, colonne});
            break;
        default:
            // Aucun opérateur mathématique reconnu, ne rien faire
            break;
    }
}

void Lexer::traiterDelimiteurs(char current, vector<Token>& tokens, int ligne, int colonne) {
    switch (current) {
        case '(': 
            tokens.push_back({TOKEN_PAREN_OUVERTE, "(", ligne, colonne}); 
            break;
        case ')': 
            tokens.push_back({TOKEN_PAREN_FERMEE, ")", ligne, colonne}); 
            break;
        case '{': 
            tokens.push_back({TOKEN_ACCOLADE_OUVERTE, "{", ligne, colonne}); 
            break;
        case '}': 
            tokens.push_back({TOKEN_ACCOLADE_FERMEE, "}", ligne, colonne}); 
            break;
        case '[': 
            tokens.push_back({TOKEN_CROCHET_OUVERT, "[", ligne, colonne}); 
            break;
        case ']': 
            tokens.push_back({TOKEN_CROCHET_FERME, "]", ligne, colonne}); 
            break;
        case '.':
            tokens.push_back({TOKEN_POINT, ".", ligne, colonne});
            break;
        case ';': 
            tokens.push_back({TOKEN_POINT_VIRGULE, ";", ligne, colonne}); 
            break;
        case ',': 
            tokens.push_back({TOKEN_VIRGULE, ",", ligne, colonne}); 
        default:
            // Aucun délimiteur reconnu, ne rien faire
            break;
    }
}

void Lexer::traiterOperateursComplexes(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne) {
    switch (current) {
        case '=': 
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_EGAL_EGAL, "==", ligne, colonne});
                pos++;
                colonne++;
            } else {
                tokens.push_back({TOKEN_EGAL, "=", ligne, colonne});
            }
            break;

        case '<':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_PLUS_PETIT_EGAL, "<=", ligne, colonne});
                pos++;
                colonne++;
            } else {
                tokens.push_back({TOKEN_PLUS_PETIT, "<", ligne, colonne});
            }
            break;

        case '>':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_PLUS_GRAND_EGAL, ">=", ligne, colonne});
                pos++;
                colonne++;
            } else {
                tokens.push_back({TOKEN_PLUS_GRAND, ">", ligne, colonne});
            }
            break;

        case '!':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_DIFFERENT, "!=", ligne, colonne});
                pos++;
                colonne++;
            } else {
                tokens.push_back({TOKEN_NON, "!", ligne, colonne});
            }
            break;

        case '&':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '&') {
                tokens.push_back({TOKEN_ET, "&&", ligne, colonne});
                pos++;
                colonne++;
            }
            break;

        case '|':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '|') {
                tokens.push_back({TOKEN_OU, "||", ligne, colonne});
                pos++;
                colonne++;
            }
            break;

        case ':':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == ':') {
                tokens.push_back({TOKEN_DEUX_POINTS, "::", ligne, colonne});
                pos++;
                colonne++;
            } else {
                tokens.push_back({TOKEN_DEUX_POINTS, ":", ligne, colonne});
            }
        default:
            break;
    }
}

void Lexer::traiterOperateursEtDelimiteurs(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int ligne, int& colonne) {
    if (current == '+' || current == '-' || current == '*' || current == '/' || current == '%') {
        traiterOperateursMathematiques(current, tokens, ligne, colonne);
    }
    else if (current == '(' || current == ')' || current == '{' || current == '}' || 
             current == '[' || current == ']' || current == '.' || current == ';' || current == ',') {
        traiterDelimiteurs(current, tokens, ligne, colonne);
    }
    else if (current == '=' || current == '<' || current == '>' || current == '!' || current == '&' || current == '|' || current == ':') {
        traiterOperateursComplexes(current, sourceCode, pos, tokens, ligne, colonne);
    }
}

auto Lexer::traiterNombreNegatifOuPositif(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, 
                                           string& motCourant, int ligne, int& colonne, int& colonneMotCourant) -> bool {
    // Vérifie si c'est un signe (+/-) suivi d'un chiffre en contexte valide
    if (motCourant.empty() && (current == '-' || current == '+') && 
        pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0 &&
        estContexteNombreNegatif(tokens)) {
        
        ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
        motCourant = "";
        colonneMotCourant = colonne;
        traiterNombre(sourceCode, pos, tokens, ligne, colonne);
        colonneMotCourant = colonne;
        return true;
    }
    return false;
}

auto Lexer::tokenizer(const string& sourceCode) -> vector<Token> 
{
    vector<Token> tokens;
    string motCourant;
    
    size_t pos = 0;
    int ligne = 1;
    int colonne = 1;
    int colonneMotCourant = 1;
    
    while (pos < sourceCode.length()) {
        char current = sourceCode[pos];

        if (current == '/' && pos + 1 < sourceCode.length() && 
            (sourceCode[pos + 1] == '/' || sourceCode[pos + 1] == '*')) {
            ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
            motCourant = "";
            colonneMotCourant = colonne;
            traiterCommentaires(sourceCode, pos);
            
            // Compter les nouvelles lignes dans le commentaire
            for (size_t i = pos; i < sourceCode.length(); i++) {
                if (sourceCode[i] == '\n') {
                    ligne++;
                    colonne = 1;
                } else if (sourceCode[i] != '\r') {
                    colonne++;
                }
                if (sourceCode[i] == '/' && i > 0 && sourceCode[i-1] == '*') {
                    pos = i + 1;
                    colonneMotCourant = colonne;
                    break;
                }
            }
            continue;
        }

        if (current == '"') {
            ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
            motCourant = "";
            colonneMotCourant = colonne;
            
            tokens.push_back({TOKEN_GUILLEMET, "\"", ligne, colonne});
            pos++;
            colonne++;

            string stringContent;
            int colonneDebutChaine = colonne;
            while (pos < sourceCode.length() && sourceCode[pos] != '"') {
                if (sourceCode[pos] == '\\' && pos + 1 < sourceCode.length()) {
                    stringContent += sourceCode[pos];
                    pos++;
                    colonne++;
                }
                stringContent += sourceCode[pos];
                pos++;
                colonne++;
            }

            if (!stringContent.empty()) {
                tokens.push_back({TOKEN_IDENTIFIANT, stringContent, ligne, colonneDebutChaine});
            }

            if (pos < sourceCode.length() && sourceCode[pos] == '"') {
                tokens.push_back({TOKEN_GUILLEMET, "\"", ligne, colonne});
                pos++;
                colonne++;
            }
            colonneMotCourant = colonne;
            continue;
        }

        if (motCourant.empty() && isdigit(current) != 0) {
            ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
            motCourant = "";
            colonneMotCourant = colonne;
            traiterNombre(sourceCode, pos, tokens, ligne, colonne);
            colonneMotCourant = colonne;
            continue;
        }

        // Gérer les nombres négatifs ou positifs
        if (motCourant.empty() && (current == '-' || current == '+') && 
            pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0 &&
            estContexteNombreNegatif(tokens)) {
            ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
            motCourant = "";
            colonneMotCourant = colonne;
            traiterNombre(sourceCode, pos, tokens, ligne, colonne);
            colonneMotCourant = colonne;
            continue;
        }

        if (isspace(current) != 0) {
            ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
            motCourant = "";
            colonneMotCourant = colonne;
            
            if (current == '\n') {
                ligne++;
                colonne = 1;
                colonneMotCourant = 1;
            } else if (current != '\r') {
                colonne++;
            }
            pos++;
            continue;
        }

        if (!motCourant.empty() && isalnum(current) == 0 && current != '_') {
            ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
            motCourant = "";
            colonneMotCourant = colonne;
        }

        if (isalnum(current) != 0 || current == '_') {
            if (motCourant.empty()) {
                colonneMotCourant = colonne;
            }
            motCourant += current;
            pos++;
            colonne++;
            continue;
        }

        traiterOperateursEtDelimiteurs(current, sourceCode, pos, tokens, ligne, colonne);
        pos++;
        colonne++;
    }

    ajouterMotCourant(motCourant, tokens, ligne, colonneMotCourant);
    tokens.push_back({TOKEN_EOF, "", ligne, colonne});

    return tokens;
}