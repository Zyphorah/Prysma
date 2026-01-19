#include "Lexer/Lexer.h"
#include <cctype>

// Fonction privée pour ajouter un mot courant à la liste de tokens
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

// Fonction privée pour traiter les opérateurs mathématiques simples
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
    }
}

// Fonction privée pour traiter les délimiteurs et ponctuation
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

// Fonction privée pour traiter les opérateurs complexes
void Lexer::traiterOperateursComplexes(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens) {
    switch (current) {
        case '=': 
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_EGAL_EGAL, "=="});
                pos++; // IMPORTANT : On consomme le 2ème '=' pour ne pas le relire
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
    // Vérifier si c'est un opérateur mathématique
    if (current == '+' || current == '-' || current == '*' || current == '/') {
        traiterOperateursMathematiques(current, tokens);
    }
    // Vérifier si c'est un délimiteur
    else if (current == '(' || current == ')' || current == '{' || current == '}' || 
             current == '[' || current == ']' || current == ';' || current == ',') {
        traiterDelimiteurs(current, tokens);
    }
    // Vérifier si c'est un opérateur complexe
    else if (current == '=' || current == '<' || current == '>' || current == '!') {
        traiterOperateursComplexes(current, sourceCode, pos, tokens);
    }
}

// Fonction principale de tokenization
vector<Token> Lexer::tokenizer(const string& sourceCode) {
    vector<Token> tokens;
    string motCourant;
    
    size_t pos = 0;
    
    while (pos < sourceCode.length()) {
        char current = sourceCode[pos];

        // Ignorer les espaces, tabulations, et nouvelles lignes
        if (isspace(current) != 0) {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
            pos++;
            continue;
        }

        // Si nous avons un mot courant et le caractère n'est pas alphanumérique, 
        // nous devons finir le mot
        if (!motCourant.empty() && isalnum(current) == 0 && current != '_') {
            ajouterMotCourant(motCourant, tokens);
            motCourant = "";
        }

        // Si c'est un caractère alphanumérique ou underscore, ajouter au mot courant
        if (isalnum(current) != 0 || current == '_') {
            motCourant += current;
            pos++;
            continue;
        }

        // Traiter les opérateurs et délimiteurs
        traiterOperateursEtDelimiteurs(current, sourceCode, pos, tokens);

        pos++;
    }

    // Ajouter le dernier mot s'il existe
    ajouterMotCourant(motCourant, tokens);

    // Ajouter le token EOF
    tokens.push_back({TOKEN_EOF, ""});

    return tokens;
}