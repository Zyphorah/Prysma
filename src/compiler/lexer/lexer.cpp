//===-- lexer.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <iostream>
#include <cctype>
#include <cstddef>
#include <set>
#include <string.h>
#include <string>

auto Lexer::isNegativeNumberContext(const vector<Token>& tokens) -> bool {
    if (tokens.empty()) {
        return true;
    }
    
    TokenType lastType = tokens.back().type;
    
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
    bool isFloat = false;
    size_t startPos = pos;
    
    if ((sourceCode[pos] == '-' || sourceCode[pos] == '+') && 
        pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0) {
        pos++;
        column++;
    }
    
    while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
        pos++;
        column++;
    }
    
    if (pos < sourceCode.length() && sourceCode[pos] == '.') {
        isFloat = true;
        pos++;
        column++;
        
        while (pos < sourceCode.length() && isdigit(sourceCode[pos]) != 0) {
            pos++;
            column++;
        }
    }
    
    llvm::StringRef val(sourceCode.data() + startPos, pos - startPos);
    if (val.empty()) std::cout << "DEBUG: handleNumber generated empty string!" << std::endl; 
    
    if (isFloat) {
        tokens.push_back({TOKEN_LIT_FLOAT, val, line, columnStart});
    } else {
        tokens.push_back({TOKEN_LIT_INT, val, line, columnStart});
    }
}

void Lexer::handleLiterals(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column) {
    if (isdigit(current) != 0) {
        handleNumber(sourceCode, pos, tokens, line, column);
    }
}

void Lexer::addCurrentWord(llvm::StringRef currentWord, vector<Token>& tokens, int line, int column) {
    if (currentWord.empty()) {
        return;
    }

    TokenType type = TOKEN_IDENTIFIER;
    for (const auto& [keyword, tokenType] : keywordsArray) {
        if (currentWord == keyword) {
            type = tokenType;
            break;
        }
    }
    Token token;
    
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
        case '+': tokens.push_back({TOKEN_PLUS, "+", line, column}); break;
        case '-': tokens.push_back({TOKEN_MINUS, "-", line, column}); break;
        case '*': tokens.push_back({TOKEN_STAR, "*", line, column}); break;
        case '/': tokens.push_back({TOKEN_SLASH, "/", line, column}); break;
        case '%': tokens.push_back({TOKEN_MODULO, "%", line, column}); break;
        default: break;
    }
}

void Lexer::handleDelimiters(char current, vector<Token>& tokens, int line, int column) {
    switch (current) {
        case '(': tokens.push_back({TOKEN_PAREN_OPEN, "(", line, column}); break;
        case ')': tokens.push_back({TOKEN_PAREN_CLOSE, ")", line, column}); break;
        case '{': tokens.push_back({TOKEN_BRACE_OPEN, "{", line, column}); break;
        case '}': tokens.push_back({TOKEN_BRACE_CLOSE, "}", line, column}); break;
        case '[': tokens.push_back({TOKEN_BRACKET_OPEN, "[", line, column}); break;
        case ']': tokens.push_back({TOKEN_BRACKET_CLOSE, "]", line, column}); break;
        case '.': tokens.push_back({TOKEN_DOT, ".", line, column}); break;
        case ';': tokens.push_back({TOKEN_SEMICOLON, ";", line, column}); break;
        case ',': tokens.push_back({TOKEN_COMMA, ",", line, column}); break;
        default: break;
    }
}

void Lexer::handleComplexOperators(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column) {
    switch (current) {
        case '=': 
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_EQUAL_EQUAL, "==", line, column});
                pos++; column++;
            } else tokens.push_back({TOKEN_EQUAL, "=", line, column});
            break;
        case '<':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_LESS_EQUAL, "<=", line, column});
                pos++; column++;
            } else tokens.push_back({TOKEN_LESS, "<", line, column});
            break;
        case '>':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_GREATER_EQUAL, ">=", line, column});
                pos++; column++;
            } else tokens.push_back({TOKEN_GREATER, ">", line, column});
            break;
        case '!':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '=') {
                tokens.push_back({TOKEN_NOT_EQUAL, "!=", line, column});
                pos++; column++;
            } else tokens.push_back({TOKEN_NOT, "!", line, column});
            break;
        case '&':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '&') {
                tokens.push_back({TOKEN_AND, "&&", line, column});
                pos++; column++;
            }
            break;
        case '|':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '|') {
                tokens.push_back({TOKEN_OR, "||", line, column});
                pos++; column++;
            }
            break;
        case ':':
            if (pos + 1 < sourceCode.length() && sourceCode[pos + 1] == ':') {
                tokens.push_back({TOKEN_COLON, "::", line, column});
                pos++; column++;
            } else tokens.push_back({TOKEN_COLON, ":", line, column});
        default: break;
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

auto Lexer::tokenize(const string& sourceCode) -> vector<Token> {
    vector<Token> tokens;
    
    size_t pos = 0;
    int line = 1;
    int column = 1;
    
    size_t startCurrentWord = string::npos;
    int columnCurrentWord = 1;

    auto pushCurrentWord = [&]() {
        if (startCurrentWord != string::npos) {
            addCurrentWord(llvm::StringRef(sourceCode.data() + startCurrentWord, pos - startCurrentWord), tokens, line, columnCurrentWord);
            startCurrentWord = string::npos;
        }
    };
    
    while (pos < sourceCode.length()) {
        char current = sourceCode[pos];

        if (current == '/' && pos + 1 < sourceCode.length() && 
            (sourceCode[pos + 1] == '/' || sourceCode[pos + 1] == '*')) {
            pushCurrentWord();
            handleComments(sourceCode, pos);
            for (size_t i = pos; i < sourceCode.length(); i++) {
                if (sourceCode[i] == '\n') {
                    line++;
                    column = 1;
                } else if (sourceCode[i] != '\r') {
                    column++;
                }
                if (sourceCode[i] == '/' && i > 0 && sourceCode[i-1] == '*') {
                    pos = i + 1;
                    break;
                }
            }
            continue;
        }

        if (current == '"') {
            pushCurrentWord();
            
            tokens.push_back({TOKEN_QUOTE, "\"", line, column});
            pos++; column++;

            int columnStartString = column;
            size_t startString = pos;
            
            while (pos < sourceCode.length() && sourceCode[pos] != '"') {
                if (sourceCode[pos] == '\\' && pos + 1 < sourceCode.length()) {
                    pos++; column++;
                }
                pos++; column++;
            }

            if (startString != pos) {
                // StringRef must point to a string that lives as long as the token array
                llvm::StringRef val(sourceCode.data() + startString, pos - startString);
                tokens.push_back({TOKEN_IDENTIFIER, val, line, columnStartString});
            }

            if (pos < sourceCode.length() && sourceCode[pos] == '"') {
                tokens.push_back({TOKEN_QUOTE, "\"", line, column});
                pos++; column++;
            }
            continue;
        }

        if (startCurrentWord == string::npos && isdigit(current) != 0) {
            handleNumber(sourceCode, pos, tokens, line, column);
            continue;
        }

        if (startCurrentWord == string::npos && (current == '-' || current == '+') && 
            pos + 1 < sourceCode.length() && isdigit(sourceCode[pos + 1]) != 0 &&
            isNegativeNumberContext(tokens)) {
            handleNumber(sourceCode, pos, tokens, line, column);
            continue;
        }

        if (isspace(current) != 0) {
            pushCurrentWord();
            if (current == '\n') {
                line++;
                column = 1;
            } else if (current != '\r') {
                column++;
            }
            pos++;
            continue;
        }

        if (startCurrentWord != string::npos && isalnum(current) == 0 && current != '_') {
            pushCurrentWord();
        }

        if (isalnum(current) != 0 || current == '_') {
            if (startCurrentWord == string::npos) {
                startCurrentWord = pos;
                columnCurrentWord = column;
            }
            pos++; column++;
            continue;
        }

        handleOperatorsAndDelimiters(current, sourceCode, pos, tokens, line, column);
        pos++; column++;
    }

    pushCurrentWord();
    tokens.push_back({TOKEN_EOF, "", line, column});

    return tokens;
}
