#include "catch.hpp"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/Lexer/TokenType.h"
#include <llvm/IR/Type.h>

using namespace std;

TEST_CASE("RegistryType - Enregistryr et recuperer multiples types", "[RegistryType]") {
    RegistryType registry;

    llvm::Type* type1 = reinterpret_cast<llvm::Type*>(0x1000);
    llvm::Type* type2 = reinterpret_cast<llvm::Type*>(0x2000);
    llvm::Type* type3 = reinterpret_cast<llvm::Type*>(0x3000);

    registry.enregistryr(TOKEN_IDENTIFIANT, type1);
    registry.enregistryr(TOKEN_LIT_INT, type2);
    registry.enregistryr(TOKEN_LIT_FLOAT, type3);

    CHECK(registry.existe(TOKEN_IDENTIFIANT));
    CHECK(registry.existe(TOKEN_LIT_INT));
    CHECK(registry.existe(TOKEN_LIT_FLOAT));

    CHECK(registry.recuperer(TOKEN_IDENTIFIANT) == type1);
    CHECK(registry.recuperer(TOKEN_LIT_INT) == type2);
    CHECK(registry.recuperer(TOKEN_LIT_FLOAT) == type3);
}

TEST_CASE("RegistryType - Type inexistant lance exception", "[RegistryType]") {
    RegistryType registry;

    CHECK_FALSE(registry.existe(TOKEN_PLUS));
    CHECK_THROWS_AS(registry.recuperer(TOKEN_PLUS), std::invalid_argument);
}

TEST_CASE("RegistryType - Surload remplace le type existant", "[RegistryType]") {
    RegistryType registry;

    llvm::Type* type1 = reinterpret_cast<llvm::Type*>(0x1000);
    llvm::Type* type2 = reinterpret_cast<llvm::Type*>(0x2000);

    registry.enregistryr(TOKEN_IDENTIFIANT, type1);
    CHECK(registry.recuperer(TOKEN_IDENTIFIANT) == type1);

    registry.enregistryr(TOKEN_IDENTIFIANT, type2);
    CHECK(registry.recuperer(TOKEN_IDENTIFIANT) == type2);
}

TEST_CASE("RegistryType - Obtenir toutes les cles", "[RegistryType]") {
    RegistryType registry;

    registry.enregistryr(TOKEN_IDENTIFIANT, reinterpret_cast<llvm::Type*>(0x1000));
    registry.enregistryr(TOKEN_LIT_INT, reinterpret_cast<llvm::Type*>(0x2000));
    registry.enregistryr(TOKEN_LIT_FLOAT, reinterpret_cast<llvm::Type*>(0x3000));
    registry.enregistryr(TOKEN_LIT_BOLEEN, reinterpret_cast<llvm::Type*>(0x4000));

    auto cles = registry.obtenirCles();
    CHECK(cles.size() == 4);
    CHECK(cles.count(TOKEN_IDENTIFIANT) == 1);
    CHECK(cles.count(TOKEN_LIT_INT) == 1);
    CHECK(cles.count(TOKEN_LIT_FLOAT) == 1);
    CHECK(cles.count(TOKEN_LIT_BOLEEN) == 1);
}

TEST_CASE("RegistryType - Enregistryr null et recuperer sans exception", "[RegistryType][SadTest]") {
    RegistryType registry;

    llvm::Type* typeValid = reinterpret_cast<llvm::Type*>(0x2000);

    registry.enregistryr(TOKEN_IDENTIFIANT, nullptr);
    registry.enregistryr(TOKEN_LIT_INT, typeValid);
    registry.enregistryr(TOKEN_LIT_FLOAT, nullptr);
    registry.enregistryr(TOKEN_LIT_BOLEEN, reinterpret_cast<llvm::Type*>(0x4000));

    CHECK(registry.existe(TOKEN_IDENTIFIANT));
    CHECK(registry.existe(TOKEN_LIT_INT));
    CHECK(registry.existe(TOKEN_LIT_FLOAT));
    CHECK(registry.existe(TOKEN_LIT_BOLEEN));

    CHECK(registry.recuperer(TOKEN_IDENTIFIANT) == nullptr);
    CHECK(registry.recuperer(TOKEN_LIT_INT) == typeValid);
    CHECK(registry.recuperer(TOKEN_LIT_FLOAT) == nullptr);
    CHECK(registry.recuperer(TOKEN_LIT_BOLEEN) == reinterpret_cast<llvm::Type*>(0x4000));

    CHECK_THROWS_AS(registry.recuperer(TOKEN_PLUS), std::invalid_argument);
    CHECK(registry.obtenirCles().size() == 4);

    registry.enregistryr(TOKEN_PLUS, nullptr);
    CHECK(registry.existe(TOKEN_PLUS));
    CHECK(registry.recuperer(TOKEN_PLUS) == nullptr);
}
