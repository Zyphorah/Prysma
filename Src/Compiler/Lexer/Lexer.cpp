#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cctype>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

auto Lexer::isNegativeNumberContext(const vector<Token>& tokens) -> bool {
    if (tokens.empty()) {
        return true;
    }
    
    TokenType lastType = tokens.back().type;
    
    // Contexts where +/- are unary operators
    static const set<TokenType> unaryOperators = {
        TOKEN_EQUAL, TOKEN_PAREN_OPEN, TOKEN_COMMA, TOKEN_BRACKET_OPEN,
        TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_MODULO,
        TOKEN_EQUAL_EQUAL, TOKEN_NOT_EQUAL, TOKEN_LESS, TOKEN_GREATER,
        TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL, TOKEN_AND, TOKEN_OR
    };
    
    return unaryOperators.count(lastType) > 0;
}

void Lexer::handleComments(const string& sourceCode, size_t& pos) {
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

void Lexer::handleNumber(const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column) {
    int columnStart = column;
    string number;
    bool isFloat = false;
    
    if ((sourceCode[pos] == '-' || sourceCode[pos] == '+') && 
        pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0) {
        number += sourceCode[pos];
        pos++;
        column++;
    }
    
    while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
        number += sourceCode[pos];
        pos++;
        column++;
    }
    
    if (pos < sourceCode.length() && sourceCode[pos] == '.') {
        isFloat = true;
        number += sourceCode[pos];
        pos++;
        column++;
        
        while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
            number += sourceCode[pos];
            pos++;
            column++;
        }
    }
    
    if (isFloat) {
        tokens.push_back({TOKEN_LIT_FLOAT, number, line, columnStart});
    } else {
        tokens.push_back({TOKEN_LIT_INT, number, line, columnStart});
    }
}

void Lexer::handleLiterals(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column) {
    if (isdigit(current) != 0) {
        handleNumber(sourceCode, pos, tokens, line, column);
    }
}

void Lexer::addCurrentWord(const string& currentWord, vector<Token>& tokens
    , int line 
    , int column 
    )
{

    if (currentWord.empty()) {
        return;
    }

    TokenType type = TOKEN_IDENTIFIER;
    for (const auto& [keyword, tokenType] : keywordsArray) {
        if (keyword == currentWord) {
            type = tokenType;
            break;
        }
    }
    Token token;
    
    // Convert booleans true/false to numeric literals
    if (type == TOKEN_TRUE) {
        token.type = TOKEN_LIT_BOOL;
        token.value = "1";
    } else if (type == TOKEN_FALSE) {
        token.type = TOKEN_LIT_BOOL;
        token.value = "0";
    } else {
        token.type = type;
        token.value = currentWord;
    }
    
    token.line = line;
    token.column = column;
    
    tokens.push_back(token);
}

void Lexer::handleMathOperators(char current, vector<Token>& tokens, int line, int column) {
    switch (current) {
        case '+': 
            tokens.push_back({TOKEN_PLUS, "+", line, column}); 
            break;
        case '-': 
            tokens.push_back({TOKEN_MINUS, "-", line, column}); 
            break;
        case '*': 
            tokens.push_back({TOKEN_STAR, "*", line, column}); 
            break;
        case '/': 
            tokens.push_back({TOKEN_SLASH, "/", line, column}); 
            break;
        case '%':
            tokens.push_back({TOKEN_MODULO, "%", line, column});
            break;
        default:
            // No recognized math operator, do nothing
            break;
    }
}

void Lexer::handleDelimiters(char current, vector<Token>& tokens, int line, int column) {
    switch (current) {
        case '(': 
            tokens.push_back({TOKEN_PAREN_OPEN, "(", line, column}); 
            break;
        case ')': 
            tokens.push_back({TOKEN_PAREN_CLOSE, ")", line, column}); 
            break;
        case '{': 
            tokens.push_back({TOKEN_BRACE_OPEN, "{", line, column}); 
            break;
        case '}': 
            tokens.push_back({TOKEN_BRACE_CLOSE, "}", line, column}); 
            break;
        case '[': 
            tokens.push_back({TOKEN_BRACKET_OPEN, "[", line, column}); 
            break;
        case ']': 
            tokens.push_back({TOKEN_BRACKET_CLOSE, "]", line, column}); 
            break;
        case '.':
            tokens.push_back({TOKEN_DOT, ".", line, column});
            break;
        case ';': 
            tokens.push_back({TOKEN_SEMICOLON, ";", line, column}); 
            break;
        case ',': 
            tokens.push_back({TOKEN_COMMA, ",", line, column}); 
        default:
            // No recognized delimiter, do nothing
            break;
    }
}

void Lexer::handleComplexOperators(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column) {
    switch (current) {
        case '=': 
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_EQUAL_EQUAL, "==", line, column});
                pos++;
                column++;
            } else {
                tokens.push_back({TOKEN_EQUAL, "=", line, column});
            }
            break;

        case '<':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_LESS_EQUAL, "<=", line, column});
                pos++;
                column++;
            } else {
                tokens.push_back({TOKEN_LESS, "<", line, column});
            }
            break;

        case '>':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_GREATER_EQUAL, ">=", line, column});
                pos++;
                column++;
            } else {
                tokens.push_back({TOKEN_GREATER, ">", line, column});
            }
            break;

        case '!':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_NOT_EQUAL, "!=", line, column});
                pos++;
                column++;
            } else {
                tokens.push_back({TOKEN_NOT, "!", line, column});
            }
            break;

        case '&':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '&') {
                tokens.push_back({TOKEN_AND, "&&", line, column});
                pos++;
                column++;
            }
            break;

        case '|':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '|') {
                tokens.push_back({TOKEN_OR, "||", line, column});
                pos++;
                column++;
            }
            break;

        case ':':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == ':') {
                tokens.push_back({TOKEN_COLON, "::", line, column});
                pos++;
                column++;
            } else {
                tokens.push_back({TOKEN_COLON, ":", line, column});
            }
        default:
            break;
    }
}

void Lexer::handleOperatorsAndDelimiters(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column) {
    if (current == '+' || current == '-' || current == '*' || current == '/' || current == '%') {
        handleMathOperators(current, tokens, line, column);
    }
    else if (current == '(' || current == ')' || current == '{' || current == '}' || 
             current == '[' || current == ']' || current == '.' || current == ';' || current == ',') {
        handleDelimiters(current, tokens, line, column);
    }
    else if (current == '=' || current == '<' || current == '>' || current == '!' || current == '&' || current == '|' || current == ':') {
        handleComplexOperators(current, sourceCode, pos, tokens, line, column);
    }
}

auto Lexer::handleNegativeOrPositiveNumber(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, 
                                           string& currentWord, int line, int& column, int& columnCurrentWord) -> bool {
    // Checks if it's a sign (+/-) followed by a digit in a valid context
    if (currentWord.empty() && (current == '-' || current == '+') && 
        pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0 &&
        isNegativeNumberContext(tokens)) {
        
        addCurrentWord(currentWord, tokens, line, columnCurrentWord);
        currentWord = "";
        columnCurrentWord = column;
        handleNumber(sourceCode, pos, tokens, line, column);
        columnCurrentWord = column;
        return true;
    }
    return false;
}

auto Lexer::tokenize(const string& sourceCode) -> vector<Token> 
{
    vector<Token> tokens;
    string currentWord;
    
    size_t pos = 0;
    int line = 1;
    int column = 1;
    int columnCurrentWord = 1;
    
    while (pos < sourceCode.length()) {
        char current = sourceCode[pos];

        if (current == '/' && pos + 1 < sourceCode.length() && 
            (sourceCode[pos + 1] == '/' || sourceCode[pos + 1] == '*')) {
            addCurrentWord(currentWord, tokens, line, columnCurrentWord);
            currentWord = "";
            columnCurrentWord = column;
            handleComments(sourceCode, pos);
            
            // Count new lines in the comment
            for (size_t i = pos; i < sourceCode.length(); i++) {
                if (sourceCode[i] == '\n') {
                    line++;
                    column = 1;
                } else if (sourceCode[i] != '\r') {
                    column++;
                }
                if (sourceCode[i] == '/' && i > 0 && sourceCode[i-1] == '*') {
                    pos = i + 1;
                    columnCurrentWord = column;
                    break;
                }
            }
            continue;
        }

        if (current == '"') {
            addCurrentWord(currentWord, tokens, line, columnCurrentWord);
            currentWord = "";
            columnCurrentWord = column;
            
            tokens.push_back({TOKEN_QUOTE, "\"", line, column});
            pos++;
            column++;

            string stringContent;
            int columnStartString = column;
            while (pos < sourceCode.length() && sourceCode[pos] != '"') {
                if (sourceCode[pos] == '\\' && pos + 1 < sourceCode.length()) {
                    stringContent += sourceCode[pos];
                    pos++;
                    column++;
                }
                stringContent += sourceCode[pos];
                pos++;
                column++;
            }

            if (!stringContent.empty()) {
                tokens.push_back({TOKEN_IDENTIFIER, stringContent, line, columnStartString});
            }

            if (pos < sourceCode.length() && sourceCode[pos] == '"') {
                tokens.push_back({TOKEN_QUOTE, "\"", line, column});
                pos++;
                column++;
            }
            columnCurrentWord = column;
            continue;
        }

        if (currentWord.empty() && isdigit(current) != 0) {
            addCurrentWord(currentWord, tokens, line, columnCurrentWord);
            currentWord = "";
            columnCurrentWord = column;
            handleNumber(sourceCode, pos, tokens, line, column);
            columnCurrentWord = column;
            continue;
        }

        // Handle negative or positive numbers
        if (currentWord.empty() && (current == '-' || current == '+') && 
            pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0 &&
            isNegativeNumberContext(tokens)) {
            addCurrentWord(currentWord, tokens, line, columnCurrentWord);
            currentWord = "";
            columnCurrentWord = column;
            handleNumber(sourceCode, pos, tokens, line, column);
            columnCurrentWord = column;
            continue;
        }

        if (isspace(current) != 0) {
            addCurrentWord(currentWord, tokens, line, columnCurrentWord);
            currentWord = "";
            columnCurrentWord = column;
            
            if (current == '\n') {
                line++;
                column = 1;
                columnCurrentWord = 1;
            } else if (current != '\r') {
                column++;
            }
            pos++;
            continue;
        }

        if (!currentWord.empty() && isalnum(current) == 0 && current != '_') {
            addCurrentWord(currentWord, tokens, line, columnCurrentWord);
            currentWord = "";
            columnCurrentWord = column;
        }

        if (isalnum(current) != 0 || current == '_') {
            if (currentWord.empty()) {
                columnCurrentWord = column;
            }
            currentWord += current;
            pos++;
            column++;
            continue;
        }

        handleOperatorsAndDelimiters(current, sourceCode, pos, tokens, line, column);
        pos++;
        column++;
    }

    addCurrentWord(currentWord, tokens, line, columnCurrentWord);
    tokens.push_back({TOKEN_EOF, "", line, column});

    return tokens;
}