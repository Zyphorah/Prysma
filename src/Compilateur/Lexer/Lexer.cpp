#include "Lexer/Lexer.h"
#include <cctype>

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

void Lexer::traiterChaine(const string& sourceCode, size_t& pos, vector<Token>& tokens) {
    if (sourceCode[pos] != '"') {
        return;
    }
    
    string valeur;
    valeur += sourceCode[pos];
    pos++;
    
    while (pos < sourceCode.length() && sourceCode[pos] != '"') {
        if (sourceCode[pos] == '\\' && pos + 1 < sourceCode.length()) {
            valeur += sourceCode[pos];
            pos++;
            valeur += sourceCode[pos];
            pos++;
        } else {
            valeur += sourceCode[pos];
            pos++;
        }
    }
    
    if (pos < sourceCode.length()) {
        valeur += sourceCode[pos];
        pos++;
    }
    
    tokens.push_back({TOKEN_LIT_CHAINE, valeur});
}

void Lexer::traiterNombre(const string& sourceCode, size_t& pos, vector<Token>& tokens) {
    string nombre;
    bool estFlottant = false;
    
    if ((sourceCode[pos] == '-' || sourceCode[pos] == '+') && 
        pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0) {
        nombre += sourceCode[pos];
        pos++;
    }
    
    while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
        nombre += sourceCode[pos];
        pos++;
    }
    
    if (pos < sourceCode.length() && sourceCode[pos] == '.') {
        estFlottant = true;
        nombre += sourceCode[pos];
        pos++;
        
        while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
            nombre += sourceCode[pos];
            pos++;
        }
    }
    
    if (estFlottant) {
        tokens.push_back({TOKEN_LIT_FLOAT, nombre});
    } else {
        tokens.push_back({TOKEN_LIT_INT, nombre});
    }
}

void Lexer::traiterLitteraux(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens) {
    if (current == '"') {
        traiterChaine(sourceCode, pos, tokens);
    } else if (isdigit(current) != 0) {
        traiterNombre(sourceCode, pos, tokens);
    }
}

void Lexer::ajouterMotCourant(const string& motCourant, vector<Token>& tokens) {
    if (motCourant.empty()) {
        return;
    }

    Token token;
    token.type = motsCles.find(motCourant) != motsCles.end() ? 
                  motsCles[motCourant] : TOKEN_IDENTIFIANT;
    token.value = motCourant;
    tokens.push_back(token);
}

void Lexer::traiterOperateursMathematiques(char current, vector<Token>& tokens) {
    switch (current) {
        case '+': 
            tokens.push_back({TOKEN_PLUS, "+"}); 
            break;
        case '-': 
            tokens.push_back({TOKEN_MOINS, "-"}); 
            break;
        case '*': 
            tokens.push_back({TOKEN_ETOILE, "*"}); 
            break;
        case '/': 
            tokens.push_back({TOKEN_SLASH, "/"}); 
            break;
        case '%':
            tokens.push_back({TOKEN_MODULO, "%"});
            break;
    }
}

void Lexer::traiterDelimiteurs(char current, vector<Token>& tokens) {
    switch (current) {
        case '(': 
            tokens.push_back({TOKEN_PAREN_OUVERTE, "("}); 
            break;
        case ')': 
            tokens.push_back({TOKEN_PAREN_FERMEE, ")"}); 
            break;
        case '{': 
            tokens.push_back({TOKEN_ACCOLADE_OUVERTE, "{"}); 
            break;
        case '}': 
            tokens.push_back({TOKEN_ACCOLADE_FERMEE, "}"}); 
            break;
        case '[': 
            tokens.push_back({TOKEN_CROCHET_OUVERT, "["}); 
            break;
        case ']': 
            tokens.push_back({TOKEN_CROCHET_FERME, "]"}); 
            break;
        case ';': 
            tokens.push_back({TOKEN_POINT_VIRGULE, ";"}); 
            break;
        case ',': 
            tokens.push_back({TOKEN_VIRGULE, ","}); 
            break;
    }
}

void Lexer::traiterOperateursComplexes(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens) {
    switch (current) {
        case '=': 
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_EGAL_EGAL, "=="});
                pos++;
            } else {
                tokens.push_back({TOKEN_EGAL, "="});
            }
            break;

        case '<':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_PLUS_PETIT_EGAL, "<="});
                pos++;
            } else {
                tokens.push_back({TOKEN_PLUS_PETIT, "<"});
            }
            break;

        case '>':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_PLUS_GRAND_EGAL, ">="});
                pos++;
            } else {
                tokens.push_back({TOKEN_PLUS_GRAND, ">"});
            }
            break;

        case '!':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_DIFFERENT, "!="});
                pos++;
            }
            break;
    }
}

void Lexer::traiterOperateursEtDelimiteurs(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens) {
    if (current == '+' || current == '-' || current == '*' || current == '/' || current == '%') {
        traiterOperateursMathematiques(current, tokens);
    }
    else if (current == '(' || current == ')' || current == '{' || current == '}' || 
             current == '[' || current == ']' || current == ';' || current == ',') {
        traiterDelimiteurs(current, tokens);
    }
    else if (current == '=' || current == '<' || current == '>' || current == '!') {
        traiterOperateursComplexes(current, sourceCode, pos, tokens);
    }
}

vector<Token> Lexer::tokenizer(const string& sourceCode) {
    vector<Token> tokens;
    string motCourant;
    
    size_t pos = 0;
    
    while (pos < sourceCode.length()) {
        char current = sourceCode[pos];

        if (current == '/' && pos + 1 < sourceCode.length() && 
            (sourceCode[pos + 1] == '/' || sourceCode[pos + 1] == '*')) {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
            traiterCommentaires(sourceCode, pos);
            continue;
        }

        if (current == '"') {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
            traiterChaine(sourceCode, pos, tokens);
            continue;
        }

        if (isdigit(current) != 0) {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
            traiterNombre(sourceCode, pos, tokens);
            continue;
        }

        if (isspace(current) != 0) {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
            pos++;
            continue;
        }

        if (!motCourant.empty() && isalnum(current) == 0 && current != '_') {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
        }

        if (isalnum(current) != 0 || current == '_') {
            motCourant += current;
            pos++;
            continue;
        }

        traiterOperateursEtDelimiteurs(current, sourceCode, pos, tokens);

        pos++;
    }

    ajouterMotCourant(motCourant, tokens);
    tokens.push_back({TOKEN_EOF, ""});

    return tokens;
}