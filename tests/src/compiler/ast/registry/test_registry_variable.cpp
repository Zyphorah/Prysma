//===-- test_registry_variable.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "catch.hpp"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/manager_error.h"
#include <string>

using namespace std;

// Pour creer un token identifiant rapidement
static Token creerToken(const string& nom, int ligne = 1, int col = 1) {
    return Token{TOKEN_IDENTIFIER, nom, ligne, col};
}

// Pour creer un symbole avec une fausse adresse unique
static Symbol createSymbol(uintptr_t adresse = 0x1000) {
    return Symbol(reinterpret_cast<llvm::Value*>(adresse), nullptr);
}

// Tests cas limites - Portee des variables (scopes et registrys)

TEST_CASE("Variable accessible dans le scope global", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("x");
    registry.registerVariable(tok, createSymbol(0x100));

    Symbol result = registry.getVariable(tok);
    CHECK(result.getAddress() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable inexistante lance exception", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("fantome");
    CHECK_THROWS_AS(registry.getVariable(tok), CompilationError);
}

TEST_CASE("Double declaration dans le meme scope lance exception", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("a");
    registry.registerVariable(tok, createSymbol(0x200));

    CHECK_THROWS_AS(registry.registerVariable(tok, createSymbol(0x300)), CompilationError);
}

TEST_CASE("Variable du scope parent visible dans le scope child", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("x");
    registry.registerVariable(tok, createSymbol(0x100));

    registry.push();

    Symbol result = registry.getVariable(tok);
    CHECK(result.getAddress() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable du scope child inaccessible apres depiler", "[RegistryVariable]") {
    RegistryVariable registry;

    registry.push();
    Token tok = creerToken("temp");
    registry.registerVariable(tok, createSymbol(0x400));

    registry.pop();

    CHECK_THROWS_AS(registry.getVariable(tok), CompilationError);
}

TEST_CASE("Shadowing - variable child masque la variable parent", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("a");
    registry.registerVariable(tok, createSymbol(0x100));

    registry.push();
    registry.registerVariable(tok, createSymbol(0x200));

    Symbol result = registry.getVariable(tok);
    CHECK(result.getAddress() == reinterpret_cast<llvm::Value*>(0x200));
}

TEST_CASE("Shadowing - variable parent restauree apres depiler", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("a");
    registry.registerVariable(tok, createSymbol(0x100));

    registry.push();
    registry.registerVariable(tok, createSymbol(0x200));
    registry.pop();

    Symbol result = registry.getVariable(tok);
    CHECK(result.getAddress() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Depiler ne supprime pas le scope global", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("globale");
    registry.registerVariable(tok, createSymbol(0x100));

    registry.pop();

    Symbol result = registry.getVariable(tok);
    CHECK(result.getAddress() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("Variable scope frere inaccessible", "[RegistryVariable]") {
    RegistryVariable registry;

    // Premier scope child
    registry.push();
    registry.registerVariable(creerToken("local1"), createSymbol(0x100));
    registry.pop();

    // Deuxieme scope child (frere)
    registry.push();
    CHECK_THROWS_AS(registry.getVariable(creerToken("local1")), CompilationError);
    registry.pop();
}

TEST_CASE("ViderTop efface le scope courant sans toucher le parent", "[RegistryVariable]") {
    RegistryVariable registry;

    registry.registerVariable(creerToken("global"), createSymbol(0x100));

    registry.push();
    registry.registerVariable(creerToken("local"), createSymbol(0x200));
    registry.clearTop();

    // Local efface
    CHECK_THROWS_AS(registry.getVariable(creerToken("local")), CompilationError);
    // Global toujours la (visible depuis le scope vide)
    CHECK(registry.getVariable(creerToken("global")).getAddress() == reinterpret_cast<llvm::Value*>(0x100));
}

TEST_CASE("ViderTop puis re-registerVariable dans le meme scope", "[RegistryVariable]") {
    RegistryVariable registry;

    Token tok = creerToken("x");
    registry.registerVariable(tok, createSymbol(0x100));
    registry.clearTop();

    // Re-registerVariable apres avoir vide
    CHECK_NOTHROW(registry.registerVariable(tok, createSymbol(0x200)));
    CHECK(registry.getVariable(tok).getAddress() == reinterpret_cast<llvm::Value*>(0x200));
}

TEST_CASE("RegistryVariable - Depilation excessive sans planter", "[RegistryVariable][SadTest]") {
    RegistryVariable registry;

    Token tok = creerToken("var");
    registry.registerVariable(tok, createSymbol(0x100));

    registry.push();
    registry.push();
    registry.pop();
    registry.pop();

    CHECK_NOTHROW(registry.getVariable(tok));
    CHECK(registry.getVariable(tok).getAddress() == reinterpret_cast<llvm::Value*>(0x100));

    CHECK_NOTHROW(registry.pop());

    CHECK_NOTHROW(registry.getVariable(tok));
}
