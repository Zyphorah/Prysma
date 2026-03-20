#include "catch.hpp"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/GestionnaireErreur.h"
#include <string>

using namespace std;

// Pour creer un token identifiant rapidement
static Token creerToken(const string& nom, int ligne = 1, int col = 1) {
    return Token{TOKEN_IDENTIFIANT, nom, ligne, col};
}

// Pour creer un symbole avec une fausse adresse unique
static Symbole creerSymbole(uintptr_t adresse = 0x1000) {
    return Symbole(reinterpret_cast<llvm::Value*>(adresse), nullptr);
}

// Tests cas limites - Portee des variables (scopes et registres)

TEST_CASE("Variable accessible dans le scope global", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("x");
    registre.enregistrer(tok, creerSymbole(0x100));

    Symbole result = registre.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable inexistante lance exception", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("fantome");
    CHECK_THROWS_AS(registre.recupererVariables(tok), ErreurCompilation);
}

TEST_CASE("Double declaration dans le meme scope lance exception", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("a");
    registre.enregistrer(tok, creerSymbole(0x200));

    CHECK_THROWS_AS(registre.enregistrer(tok, creerSymbole(0x300)), ErreurCompilation);
}

TEST_CASE("Variable du scope parent visible dans le scope enfant", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("x");
    registre.enregistrer(tok, creerSymbole(0x100));

    registre.piler();

    Symbole result = registre.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable du scope enfant inaccessible apres depiler", "[RegistreVariable]") {
    RegistreVariable registre;

    registre.piler();
    Token tok = creerToken("temp");
    registre.enregistrer(tok, creerSymbole(0x400));

    registre.depiler();

    CHECK_THROWS_AS(registre.recupererVariables(tok), ErreurCompilation);
}

TEST_CASE("Shadowing - variable enfant masque la variable parent", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("a");
    registre.enregistrer(tok, creerSymbole(0x100));

    registre.piler();
    registre.enregistrer(tok, creerSymbole(0x200));

    Symbole result = registre.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x200));
}

TEST_CASE("Shadowing - variable parent restauree apres depiler", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("a");
    registre.enregistrer(tok, creerSymbole(0x100));

    registre.piler();
    registre.enregistrer(tok, creerSymbole(0x200));
    registre.depiler();

    Symbole result = registre.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Depiler ne supprime pas le scope global", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("globale");
    registre.enregistrer(tok, creerSymbole(0x100));

    registre.depiler();

    Symbole result = registre.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable scope frere inaccessible", "[RegistreVariable]") {
    RegistreVariable registre;

    // Premier scope enfant
    registre.piler();
    registre.enregistrer(creerToken("local1"), creerSymbole(0x100));
    registre.depiler();

    // Deuxieme scope enfant (frere)
    registre.piler();
    CHECK_THROWS_AS(registre.recupererVariables(creerToken("local1")), ErreurCompilation);
    registre.depiler();
}

TEST_CASE("ViderTop efface le scope courant sans toucher le parent", "[RegistreVariable]") {
    RegistreVariable registre;

    registre.enregistrer(creerToken("global"), creerSymbole(0x100));

    registre.piler();
    registre.enregistrer(creerToken("local"), creerSymbole(0x200));
    registre.viderTop();

    // Local efface
    CHECK_THROWS_AS(registre.recupererVariables(creerToken("local")), ErreurCompilation);
    // Global toujours la (visible depuis le scope vide)
    CHECK(registre.recupererVariables(creerToken("global")).getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("ViderTop puis re-enregistrer dans le meme scope", "[RegistreVariable]") {
    RegistreVariable registre;

    Token tok = creerToken("x");
    registre.enregistrer(tok, creerSymbole(0x100));
    registre.viderTop();

    // Re-enregistrer apres avoir vide
    CHECK_NOTHROW(registre.enregistrer(tok, creerSymbole(0x200)));
    CHECK(registre.recupererVariables(tok).getAdresse() == reinterpret_cast<llvm::Value*>(0x200));
}

TEST_CASE("RegistreVariable - Depilation excessive sans planter", "[RegistreVariable][SadTest]") {
    RegistreVariable registre;

    Token tok = creerToken("var");
    registre.enregistrer(tok, creerSymbole(0x100));

    registre.piler();
    registre.piler();
    registre.depiler();
    registre.depiler();

    CHECK_NOTHROW(registre.recupererVariables(tok));
    CHECK(registre.recupererVariables(tok).getAdresse() == reinterpret_cast<llvm::Value*>(0x100));

    CHECK_NOTHROW(registre.depiler());

    CHECK_NOTHROW(registre.recupererVariables(tok));
}
