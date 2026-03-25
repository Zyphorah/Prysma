#include "catch.hpp"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/ManagerError.h"
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

// Tests cas limites - Portee des variables (scopes et registrys)

TEST_CASE("Variable accessible dans le scope global", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("x");
    registry.enregistryr(tok, creerSymbole(0x100));

    Symbole result = registry.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable inexistante lance exception", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("fantome");
    CHECK_THROWS_AS(registry.recupererVariables(tok), ErrorCompilation);
}

TEST_CASE("Double declaration dans le meme scope lance exception", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("a");
    registry.enregistryr(tok, creerSymbole(0x200));

    CHECK_THROWS_AS(registry.enregistryr(tok, creerSymbole(0x300)), ErrorCompilation);
}

TEST_CASE("Variable du scope parent visible dans le scope child", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("x");
    registry.enregistryr(tok, creerSymbole(0x100));

    registry.piler();

    Symbole result = registry.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable du scope child inaccessible apres depiler", "[RegistryVariable]") {
    RegistryVariable registry;

    registry.piler();
    Token tok = creerToken("temp");
    registry.enregistryr(tok, creerSymbole(0x400));

    registry.depiler();

    CHECK_THROWS_AS(registry.recupererVariables(tok), ErrorCompilation);
}

TEST_CASE("Shadowing - variable child masque la variable parent", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("a");
    registry.enregistryr(tok, creerSymbole(0x100));

    registry.piler();
    registry.enregistryr(tok, creerSymbole(0x200));

    Symbole result = registry.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x200));
}

TEST_CASE("Shadowing - variable parent restauree apres depiler", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("a");
    registry.enregistryr(tok, creerSymbole(0x100));

    registry.piler();
    registry.enregistryr(tok, creerSymbole(0x200));
    registry.depiler();

    Symbole result = registry.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Depiler ne supprime pas le scope global", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("globale");
    registry.enregistryr(tok, creerSymbole(0x100));

    registry.depiler();

    Symbole result = registry.recupererVariables(tok);
    CHECK(result.getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable scope frere inaccessible", "[RegistryVariable]") {
    RegistryVariable registry;

    // Premier scope child
    registry.piler();
    registry.enregistryr(creerToken("local1"), creerSymbole(0x100));
    registry.depiler();

    // Deuxieme scope child (frere)
    registry.piler();
    CHECK_THROWS_AS(registry.recupererVariables(creerToken("local1")), ErrorCompilation);
    registry.depiler();
}

TEST_CASE("ViderTop efface le scope courant sans toucher le parent", "[RegistryVariable]") {
    RegistryVariable registry;

    registry.enregistryr(creerToken("global"), creerSymbole(0x100));

    registry.piler();
    registry.enregistryr(creerToken("local"), creerSymbole(0x200));
    registry.viderTop();

    // Local efface
    CHECK_THROWS_AS(registry.recupererVariables(creerToken("local")), ErrorCompilation);
    // Global toujours la (visible depuis le scope vide)
    CHECK(registry.recupererVariables(creerToken("global")).getAdresse() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("ViderTop puis re-enregistryr dans le meme scope", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("x");
    registry.enregistryr(tok, creerSymbole(0x100));
    registry.viderTop();

    // Re-enregistryr apres avoir vide
    CHECK_NOTHROW(registry.enregistryr(tok, creerSymbole(0x200)));
    CHECK(registry.recupererVariables(tok).getAdresse() == reinterpret_cast<llvm::Value*>(0x200));
}

TEST_CASE("RegistryVariable - Depilation excessive sans planter", "[RegistryVariable][SadTest]") {
    RegistryVariable registry;

    Token tok = creerToken("var");
    registry.enregistryr(tok, creerSymbole(0x100));

    registry.piler();
    registry.piler();
    registry.depiler();
    registry.depiler();

    CHECK_NOTHROW(registry.recupererVariables(tok));
    CHECK(registry.recupererVariables(tok).getAdresse() == reinterpret_cast<llvm::Value*>(0x100));

    CHECK_NOTHROW(registry.depiler());

    CHECK_NOTHROW(registry.recupererVariables(tok));
}
