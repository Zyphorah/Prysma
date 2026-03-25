#include "Compiler/Lexer/TokenType.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Compiler/Lexer/Lexer.h"
#include <string>
#include <vector>
#include <random>

using namespace std;

TEST_CASE("Tester Lexer Identifiants", "[Lexer]") {
    Lexer lexer;
    string code = "x y z variable_test";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens.size() == 5);
    CHECK(tokens[0].value == "x");
    CHECK(tokens[1].value == "y");
    CHECK(tokens[2].value == "z");
    CHECK(tokens[3].value == "variable_test");
}

TEST_CASE("Tester Lexer Nombres", "[Lexer]") {
    Lexer lexer;
    string code = "42 3.14";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].type == TOKEN_LIT_INT);
    CHECK(tokens[0].value == "42");
    CHECK(tokens[1].type == TOKEN_LIT_FLOAT);
    CHECK(tokens[1].value == "3.14");
}

TEST_CASE("Tester Lexer Chaines de caracteres", "[Lexer]") {
    Lexer lexer;
    string code = R"("Salut")";
    vector<Token> tokens = lexer.tokenizer(code);

    // Le lexer produit : GUILLEMET, IDENTIFIANT(contenu), GUILLEMET, EOF
    CHECK(tokens[0].type == TOKEN_GUILLEMET);
    CHECK(tokens[1].type == TOKEN_IDENTIFIANT);
    CHECK(tokens[1].value == "Salut");
    CHECK(tokens[2].type == TOKEN_GUILLEMET);
}

TEST_CASE("Tester Lexer Mots-cles", "[Lexer]") {
    Lexer lexer;
    string code = "fn if else while for return dec aff ref unref include";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].type == TOKEN_FONCTION);
    CHECK(tokens[1].type == TOKEN_SI);
    CHECK(tokens[2].type == TOKEN_SINON);
    CHECK(tokens[3].type == TOKEN_TANT_QUE);
    CHECK(tokens[4].type == TOKEN_POUR);
    CHECK(tokens[5].type == TOKEN_RETOUR);
    CHECK(tokens[6].type == TOKEN_DEC);
    CHECK(tokens[7].type == TOKEN_AFF);
    CHECK(tokens[8].type == TOKEN_REF);
    CHECK(tokens[9].type == TOKEN_UNREF);
    CHECK(tokens[10].type == TOKEN_INCLUDE);
}

TEST_CASE("Tester Lexer Operateurs mathematiques", "[Lexer]") {
    Lexer lexer;
    string code = "a + b - c * d / e % f";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[1].type == TOKEN_PLUS);
    CHECK(tokens[3].type == TOKEN_MOINS);
    CHECK(tokens[5].type == TOKEN_ETOILE);
    CHECK(tokens[7].type == TOKEN_SLASH);
    CHECK(tokens[9].type == TOKEN_MODULO);
}

TEST_CASE("Tester Lexer Delimiteurs", "[Lexer]") {
    Lexer lexer;
    string code = "( ) { } [ ] ; ,";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].type == TOKEN_PAREN_OUVERTE);
    CHECK(tokens[1].type == TOKEN_PAREN_FERMEE);
    CHECK(tokens[2].type == TOKEN_ACCOLADE_OUVERTE);
    CHECK(tokens[3].type == TOKEN_ACCOLADE_FERMEE);
    CHECK(tokens[4].type == TOKEN_CROCHET_OUVERT);
    CHECK(tokens[5].type == TOKEN_CROCHET_FERME);
    CHECK(tokens[6].type == TOKEN_POINT_VIRGULE);
    CHECK(tokens[7].type == TOKEN_VIRGULE);
}

TEST_CASE("Tester Lexer Commentaires ligne", "[Lexer]") {
    Lexer lexer;
    string code = "x // ceci est un commentaire\ny";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].value == "x");
    CHECK(tokens[1].value == "y");
}

TEST_CASE("Tester Lexer Commentaires bloc", "[Lexer]") {
    Lexer lexer;
    string code = "a /* commentaire bloc */ b";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].value == "a");
    CHECK(tokens[1].value == "b");
}

TEST_CASE("Tester Lexer Booleans", "[Lexer]") {
    Lexer lexer;
    string code = "true false";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].type == TOKEN_LIT_BOLEEN);
    CHECK(tokens[0].value == "1");
    CHECK(tokens[1].type == TOKEN_LIT_BOLEEN);
    CHECK(tokens[1].value == "0");
}

TEST_CASE("Tester Lexer Code Vide", "[Lexer]") {
    Lexer lexer;
    string code;
    vector<Token> tokens = lexer.tokenizer(code);

    // Seul le token EOF devrait etre present
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Operateurs composites", "[Lexer]") {
    Lexer lexer;
    string code = "a == b != c >= d <= e && f || g";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[1].type == TOKEN_EGAL_EGAL);
    CHECK(tokens[3].type == TOKEN_DIFFERENT);
    CHECK(tokens[5].type == TOKEN_PLUS_GRAND_EGAL);
    CHECK(tokens[7].type == TOKEN_PLUS_PETIT_EGAL);
    CHECK(tokens[9].type == TOKEN_ET);
    CHECK(tokens[11].type == TOKEN_OU);
}

TEST_CASE("Tester Lexer Lignes et colonnes", "[Lexer]") {
    Lexer lexer;
    string code = "a\nb\nc";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].ligne == 1);
    CHECK(tokens[1].ligne == 2);
    CHECK(tokens[2].ligne == 3);
}

TEST_CASE("Tester Lexer Tokens sans espaces", "[Lexer]") {
    Lexer lexer;
    string code = "a+b-c*d/e";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].value == "a");
    CHECK(tokens[1].type == TOKEN_PLUS);
    CHECK(tokens[2].value == "b");
    CHECK(tokens[3].type == TOKEN_MOINS);
    CHECK(tokens[4].value == "c");
}

// Cas non functionnel sad test pour verifier que le lexer ne plante pas et termine correctement avec EOF

TEST_CASE("Tester Lexer Commentaire bloc non ferme", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "a /* commentaire non ferme";
    vector<Token> tokens = lexer.tokenizer(code);


    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Nombre flottant invalide", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "3.14.15 2.";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Identifiant commence par chiffre", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "123abc 456_var";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens[0].type == TOKEN_LIT_INT);
    CHECK(tokens[0].value == "123");
    REQUIRE(tokens.size() >= 2);
    CHECK(tokens[1].type == TOKEN_IDENTIFIANT);
}

TEST_CASE("Tester Lexer Caracteres speciaux non reconnues", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = "a @ b # c $ d";
    vector<Token> tokens = lexer.tokenizer(code);

    CHECK(tokens.size() > 0);
    CHECK(tokens[tokens.size() - 1].type == TOKEN_EOF);
}

TEST_CASE("Tester Lexer Chaine de caracteres non fermee", "[Lexer][SadTest]") {
    Lexer lexer;
    string code = R"("Chaîne non fermée)";
    vector<Token> tokens = lexer.tokenizer(code);

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
        vector<Token> tokens = lexer.tokenizer(garbage);
        
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
