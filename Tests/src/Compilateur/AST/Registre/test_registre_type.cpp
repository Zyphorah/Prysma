#include "catch.hpp"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm/IR/Type.h>

using namespace std;

TEST_CASE("RegistreType - Enregistrer et recuperer multiples types", "[RegistreType]") {
    RegistreType registre;

    llvm::Type* type1 = reinterpret_cast<llvm::Type*>(0x1000);
    llvm::Type* type2 = reinterpret_cast<llvm::Type*>(0x2000);
    llvm::Type* type3 = reinterpret_cast<llvm::Type*>(0x3000);

    registre.enregistrer(TOKEN_IDENTIFIANT, type1);
    registre.enregistrer(TOKEN_LIT_INT, type2);
    registre.enregistrer(TOKEN_LIT_FLOAT, type3);

    CHECK(registre.existe(TOKEN_IDENTIFIANT));
    CHECK(registre.existe(TOKEN_LIT_INT));
    CHECK(registre.existe(TOKEN_LIT_FLOAT));

    CHECK(registre.recuperer(TOKEN_IDENTIFIANT) == type1);
    CHECK(registre.recuperer(TOKEN_LIT_INT) == type2);
    CHECK(registre.recuperer(TOKEN_LIT_FLOAT) == type3);
}

TEST_CASE("RegistreType - Type inexistant lance exception", "[RegistreType]") {
    RegistreType registre;

    CHECK_FALSE(registre.existe(TOKEN_PLUS));
    CHECK_THROWS_AS(registre.recuperer(TOKEN_PLUS), std::invalid_argument);
}

TEST_CASE("RegistreType - Surcharge remplace le type existant", "[RegistreType]") {
    RegistreType registre;

    llvm::Type* type1 = reinterpret_cast<llvm::Type*>(0x1000);
    llvm::Type* type2 = reinterpret_cast<llvm::Type*>(0x2000);

    registre.enregistrer(TOKEN_IDENTIFIANT, type1);
    CHECK(registre.recuperer(TOKEN_IDENTIFIANT) == type1);

    registre.enregistrer(TOKEN_IDENTIFIANT, type2);
    CHECK(registre.recuperer(TOKEN_IDENTIFIANT) == type2);
}

TEST_CASE("RegistreType - Obtenir toutes les cles", "[RegistreType]") {
    RegistreType registre;

    registre.enregistrer(TOKEN_IDENTIFIANT, reinterpret_cast<llvm::Type*>(0x1000));
    registre.enregistrer(TOKEN_LIT_INT, reinterpret_cast<llvm::Type*>(0x2000));
    registre.enregistrer(TOKEN_LIT_FLOAT, reinterpret_cast<llvm::Type*>(0x3000));
    registre.enregistrer(TOKEN_LIT_BOLEEN, reinterpret_cast<llvm::Type*>(0x4000));

    auto cles = registre.obtenirCles();
    CHECK(cles.size() == 4);
    CHECK(cles.count(TOKEN_IDENTIFIANT) == 1);
    CHECK(cles.count(TOKEN_LIT_INT) == 1);
    CHECK(cles.count(TOKEN_LIT_FLOAT) == 1);
    CHECK(cles.count(TOKEN_LIT_BOLEEN) == 1);
}

TEST_CASE("RegistreType - Enregistrer null et recuperer sans exception", "[RegistreType][SadTest]") {
    RegistreType registre;

    llvm::Type* typeValid = reinterpret_cast<llvm::Type*>(0x2000);

    registre.enregistrer(TOKEN_IDENTIFIANT, nullptr);
    registre.enregistrer(TOKEN_LIT_INT, typeValid);
    registre.enregistrer(TOKEN_LIT_FLOAT, nullptr);
    registre.enregistrer(TOKEN_LIT_BOLEEN, reinterpret_cast<llvm::Type*>(0x4000));

    CHECK(registre.existe(TOKEN_IDENTIFIANT));
    CHECK(registre.existe(TOKEN_LIT_INT));
    CHECK(registre.existe(TOKEN_LIT_FLOAT));
    CHECK(registre.existe(TOKEN_LIT_BOLEEN));

    CHECK(registre.recuperer(TOKEN_IDENTIFIANT) == nullptr);
    CHECK(registre.recuperer(TOKEN_LIT_INT) == typeValid);
    CHECK(registre.recuperer(TOKEN_LIT_FLOAT) == nullptr);
    CHECK(registre.recuperer(TOKEN_LIT_BOLEEN) == reinterpret_cast<llvm::Type*>(0x4000));

    CHECK_THROWS_AS(registre.recuperer(TOKEN_PLUS), std::invalid_argument);
    CHECK(registre.obtenirCles().size() == 4);

    registre.enregistrer(TOKEN_PLUS, nullptr);
    CHECK(registre.existe(TOKEN_PLUS));
    CHECK(registre.recuperer(TOKEN_PLUS) == nullptr);
}
