//===-- token_categories.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E
#define F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E

#include "compiler/lexer/token_type.h"

constexpr auto isOperator(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_STAR:
        case TOKEN_SLASH:
        case TOKEN_MODULO:
            return true;
        default:
            return false;
    }
}

constexpr auto isOperand(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_LIT_INT:
        case TOKEN_LIT_FLOAT:
        case TOKEN_LIT_BOOL:
        case TOKEN_IDENTIFIER:
        case TOKEN_UNREF:
        case TOKEN_REF:
            return true;
        default:
            return false;
    }
}

constexpr auto isComparisonOperator(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_LESS:
        case TOKEN_GREATER:
        case TOKEN_LESS_EQUAL:
        case TOKEN_GREATER_EQUAL:
        case TOKEN_EQUAL_EQUAL:
        case TOKEN_NOT_EQUAL:
            return true;
        default:
            return false;
    }
}

constexpr auto isType(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_STRING:
        case TOKEN_TYPE_BOOL:
        case TOKEN_TYPE_VOID:
        case TOKEN_TYPE_CHAR:
        case TOKEN_TYPE_INT64:
        case TOKEN_TYPE_INT32:
        case TOKEN_TYPE_PTR:
        case TOKEN_IDENTIFIER:
            return true;
        default:
            return false;
    }
}

constexpr auto isKeyword(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_FUNCTION:
        case TOKEN_IF:
        case TOKEN_ELSE:
        case TOKEN_WHILE:
        case TOKEN_FOR:
        case TOKEN_RETURN:
        case TOKEN_SCOPE:
        case TOKEN_ASSIGN:
        case TOKEN_DECL:
            return true;
        default:
            return false;
    }
}

constexpr auto isStructure(TokenType type) noexcept -> bool {
    switch (type) {
        case TOKEN_PAREN_OPEN:
        case TOKEN_PAREN_CLOSE:
        case TOKEN_BRACE_OPEN:
        case TOKEN_BRACE_CLOSE:
        case TOKEN_BRACKET_OPEN:
        case TOKEN_BRACKET_CLOSE:
        case TOKEN_DOT:
        case TOKEN_SEMICOLON:
        case TOKEN_COMMA:
            return true;
        default:
            return false;
    }
}

#endif /* F8BDAEE4_757F_4EA2_8A22_AEAA9D20C58E */
