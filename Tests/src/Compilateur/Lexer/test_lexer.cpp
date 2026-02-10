#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Compilateur/Lexer/Lexer.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void afficherTokens(const vector<Token>& tokens) {
    cout << "\n--- Tokens générés ---" << endl;
    for (size_t i = 0; i < tokens.size(); ++i) {
        cout << "[" << i << "] Type: " << tokens[i].type 
             << " | Valeur: \"" << tokens[i].value 
             << "\" | Ligne: " << tokens[i].ligne 
             << " | Colonne: " << tokens[i].colonne << endl;
    }
}

void testerLexerIdentifiants() {
    cout << "\n Test 1 Identifiants simples" << endl;
    Lexer lexer;
    string code = "x y z variable_test";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() == 5);
    CHECK(tokens[0].value == "x");
    CHECK(tokens[1].value == "y");
    CHECK(tokens[2].value == "z");
    CHECK(tokens[3].value == "variable_test");
    cout << "Identifiants correctement tokenizés" << endl;
}

void testerLexerNombres() {
    cout << "\n Test 2 Nombres entiers et flottants" << endl;
    Lexer lexer;
    string code = "42 -15 3.14 -2.5";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() == 7);
    CHECK(tokens[0].value == "42");
    CHECK(tokens[1].value == "-");
    CHECK(tokens[2].value == "15");
    CHECK(tokens[3].value == "3.14");
    cout << "Nombres correctement tokenizés" << endl;
}

void testerLexerChaines() {
    cout << "\n Test 3 Chaînes de caractères" << endl;
    Lexer lexer;
    string code = "\"hello\" \"world with spaces\"";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() == 3);
    CHECK(tokens[0].value == "\"hello\"");
    CHECK(tokens[1].value == "\"world with spaces\"");
    cout << "Chaînes correctement tokenizées" << endl;
}

void testerLexerMotsCles() {
    cout << "\n Test 4 Mots-clés du langage Prysma" << endl;
    Lexer lexer;
    string code = "fn if else while for return true false print";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() == 10);
    CHECK(tokens[0].type == TOKEN_FONCTION);
    CHECK(tokens[1].type == TOKEN_SI);
    CHECK(tokens[2].type == TOKEN_SINON);
    cout << "Mots-clés correctement reconnus" << endl;
}

void testerLexerOperateurs() {
    cout << "\n Test 5 Opérateurs mathématiques et de comparaison" << endl;
    Lexer lexer;
    string code = "a + b - c * d / e == f != g <= h >= i";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() > 7);
    CHECK(tokens[1].type == TOKEN_PLUS);
    CHECK(tokens[3].type == TOKEN_MOINS);
    CHECK(tokens[5].type == TOKEN_ETOILE);
    CHECK(tokens[7].type == TOKEN_SLASH);
    cout << "Opérateurs correctement tokenizés" << endl;
}

void testerLexerDelimiteurs() {
    cout << "\n Test 6 Délimiteurs et ponctuation" << endl;
    Lexer lexer;
    string code = "( ) { } [ ] ; ,";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() > 3);
    CHECK(tokens[0].type == TOKEN_PAREN_OUVERTE);
    CHECK(tokens[1].type == TOKEN_PAREN_FERMEE);
    CHECK(tokens[2].type == TOKEN_ACCOLADE_OUVERTE);
    CHECK(tokens[3].type == TOKEN_ACCOLADE_FERMEE);
    cout << "Délimiteurs correctement tokenizés" << endl;
}

void testerLexerCommentaires() {
    cout << "\n Test 7 Commentaires" << endl;
    Lexer lexer;
    string code = "x // ceci est un commentaire\ny";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() >= 2);
    CHECK(tokens[0].value == "x");
    CHECK(tokens[1].value == "y");
    cout << "Commentaires correctement ignorés" << endl;
}

void testerLexerCodeComplet() {
    cout << "\n test 8 Code complet et mixte" << endl;
    Lexer lexer;
    string code = "fn main(x: int) { y = 42 + 3.14; }";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() > 0);
    CHECK(tokens[0].type == TOKEN_FONCTION);
    CHECK(tokens[0].ligne == 1);
    CHECK(tokens[0].colonne == 1);
    cout << "Code complet correctement tokenizé" << endl;
}

void testerLexerBooleans() {
    cout << "\n Test 9 Littéraux booléens" << endl;
    Lexer lexer;
    string code = "true false";
    vector<Token> tokens = lexer.tokenizer(code);
    afficherTokens(tokens);
    
    REQUIRE(tokens.size() >= 2);
    CHECK(tokens[0].type == TOKEN_LIT_BOLEEN);
    CHECK(tokens[0].value == "1");
    CHECK(tokens[1].type == TOKEN_LIT_BOLEEN);
    CHECK(tokens[1].value == "0");
    cout << "Booléens correctement tokenizés" << endl;
}

void testerTousLesTestsLexer() {
    testerLexerIdentifiants();
    testerLexerNombres();
    testerLexerChaines();
    testerLexerMotsCles();
    testerLexerOperateurs();
    testerLexerDelimiteurs();
    testerLexerCommentaires();
    testerLexerCodeComplet();
    testerLexerBooleans();
}

TEST_CASE("Execution Suite Lexer", "[lexer_all]") {
    testerTousLesTestsLexer();
}