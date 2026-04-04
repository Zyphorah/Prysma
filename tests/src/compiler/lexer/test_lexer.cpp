//===-- test_lexer.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/lexer/token_type.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "compiler/lexer/lexer.h"
#include <string>
#include <vector>
#include <random>

using namespace std;

TEST_CASE("Tester Lexer Identifiants", "[Lexer]") {
    Lexer lexer;
    string code = "x y z variable_test";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens.size() == 5);
    CHECK(tokens[0].value == "x");
    CHECK(tokens[1].value == "y");
    CHECK(tokens[2].value == "z");
    CHECK(tokens[3].value == "variable_test");
}

TEST_CASE("Tester Lexer Nombres", "[Lexer]") {
    Lexer lexer;
    string code = "42 3.14";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].type == TOKEN_LIT_INT);
    CHECK(tokens[0].value == "42");
    CHECK(tokens[1].type == TOKEN_LIT_FLOAT);
    CHECK(tokens[1].value == "3.14");
}

TEST_CASE("Tester Lexer Chaines de caracteres", "[Lexer]") {
    Lexer lexer;
    string code = R"("Salut")";
    vector<Token> tokens = lexer.tokenize(code);

    // Le lexer produit : GUILLEMET, IDENTIFIANT(contenu), GUILLEMET, EOF
    CHECK(tokens[0].type == TOKEN_QUOTE);
    CHECK(tokens[1].type == TOKEN_IDENTIFIER);
    CHECK(tokens[1].value == "Salut");
    CHECK(tokens[2].type == TOKEN_QUOTE);
}

TEST_CASE("Tester Lexer Mots-cles", "[Lexer]") {
    Lexer lexer;
    string code = "fn if else while for return dec aff ref unref include";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].type == TOKEN_FUNCTION);
    CHECK(tokens[1].type == TOKEN_IF);
    CHECK(tokens[2].type == TOKEN_ELSE);
    CHECK(tokens[3].type == TOKEN_WHILE);
    CHECK(tokens[4].type == TOKEN_FOR);
    CHECK(tokens[5].type == TOKEN_RETURN);
    CHECK(tokens[6].type == TOKEN_DECL);
    CHECK(tokens[7].type == TOKEN_ASSIGN);
    CHECK(tokens[8].type == TOKEN_REF);
    CHECK(tokens[9].type == TOKEN_UNREF);
    CHECK(tokens[10].type == TOKEN_INCLUDE);
}

TEST_CASE("Tester Lexer Operateurs mathematiques", "[Lexer]") {
    Lexer lexer;
    string code = "a + b - c * d / e % f";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[1].type == TOKEN_PLUS);
    CHECK(tokens[3].type == TOKEN_MINUS);
    CHECK(tokens[5].type == TOKEN_STAR);
    CHECK(tokens[7].type == TOKEN_SLASH);
    CHECK(tokens[9].type == TOKEN_MODULO);
}

TEST_CASE("Tester Lexer Delimiteurs", "[Lexer]") {
    Lexer lexer;
    string code = "( ) { } [ ] ; ,";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].type == TOKEN_PAREN_OPEN);
    CHECK(tokens[1].type == TOKEN_PAREN_CLOSE);
    CHECK(tokens[2].type == TOKEN_BRACE_OPEN);
    CHECK(tokens[3].type == TOKEN_BRACE_CLOSE);
    CHECK(tokens[4].type == TOKEN_BRACKET_OPEN);
    CHECK(tokens[5].type == TOKEN_BRACKET_CLOSE);
    CHECK(tokens[6].type == TOKEN_SEMICOLON);
    CHECK(tokens[7].type == TOKEN_COMMA);
}

TEST_CASE("Tester Lexer Commentaires ligne", "[Lexer]") {
    Lexer lexer;
    string code = "x // ceci est un commentaire\ny";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].value == "x");
    CHECK(tokens[1].value == "y");
}

TEST_CASE("Tester Lexer Commentaires bloc", "[Lexer]") {
    Lexer lexer;
    string code = "a /* commentaire bloc */ b";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].value == "a");
    CHECK(tokens[1].value == "b");
}

TEST_CASE("Tester Lexer Booleans", "[Lexer]") {
    Lexer lexer;
    string code = "true false";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].type == TOKEN_LIT_BOOL);
    CHECK(tokens[0].value == "1");
    CHECK(tokens[1].type == TOKEN_LIT_BOOL);
    CHECK(tokens[1].value == "0");
}

TEST_CASE("Tester Lexer Code Vide", "[Lexer]") {
    Lexer lexer;
    string code;
    vector<Token> tokens = lexer.tokenize(code);

    // Seul le token EOF devrait etre present
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Operateurs composites", "[Lexer]") {
    Lexer lexer;
    string code = "a == b != c >= d <= e && f || g";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[1].type == TOKEN_EQUAL_EQUAL);
    CHECK(tokens[3].type == TOKEN_NOT_EQUAL);
    CHECK(tokens[5].type == TOKEN_GREATER_EQUAL);
    CHECK(tokens[7].type == TOKEN_LESS_EQUAL);
    CHECK(tokens[9].type == TOKEN_AND);
    CHECK(tokens[11].type == TOKEN_OR);
}

TEST_CASE("Tester Lexer Lignes et colonnes", "[Lexer]") {
    Lexer lexer;
    string code = "a\nb\nc";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].line == 1);
    CHECK(tokens[1].line == 2);
    CHECK(tokens[2].line == 3);
}

TEST_CASE("Tester Lexer Tokens sans espaces", "[Lexer]") {
    Lexer lexer;
    string code = "a+b-c*d/e";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].value == "a");
    CHECK(tokens[1].type == TOKEN_PLUS);
    CHECK(tokens[2].value == "b");
    CHECK(tokens[3].type == TOKEN_MINUS);
    CHECK(tokens[4].value == "c");
}

// Cas non functionnel sad test pour verifier que le lexer ne plante pas et termine correctement avec EOF

TEST_CASE("Tester Lexer Commentaire bloc non ferme", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "a /* commentaire non ferme";
    vector<Token> tokens = lexer.tokenize(code);


    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Nombre flottant invalide", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "3.14.15 2.";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Identifiant commence par chiffre", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "123abc 456_var";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens[0].type == TOKEN_LIT_INT);
    CHECK(tokens[0].value == "123");
    REQUIRE(tokens.size() >= 2);
    CHECK(tokens[1].type == TOKEN_IDENTIFIER);
}

TEST_CASE("Tester Lexer Caracteres speciaux non reconnues", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "a @ b # c $ d";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Chaine de caracteres non fermee", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = R"("Chaîne non fermée)";
    vector<Token> tokens = lexer.tokenize(code);

    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}


TEST_CASE("Tester Lexer Fuzzing - Garbage aleatoire", "[Lexer][SadTest][Fuzzing]") {
    Lexer lexer;
    
    // Generateur aleatoire
    mt19937 gen(12345); // Seed fixe pour reproductibilite
    uniform_int_distribution<> dis(0, 255);
    
    // Generer du garbage aleatoire
    string garbage;
    for (int i = 0; i < 5000; ++i) {
        garbage += static_cast<char>(dis(gen));
    }
    
    // Il doit toujours returnner un vecteur avec au moins EOF
    try {
        vector<Token> tokens = lexer.tokenize(garbage);
        
        // Verifications robustes
        CHECK(tokens.size() > 0);
        CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
        
        // S'assurer que le dernier token avant EOF est valide
        if (tokens.size() > 1) {
            CHECK(tokens[tokens.size() - 2].type != TOKEN_EOF);
        }
    } catch (...) {
        FAIL("Le lexer a lance une exception sur du garbage - Ne doit jamais crasher!");
    }
}
