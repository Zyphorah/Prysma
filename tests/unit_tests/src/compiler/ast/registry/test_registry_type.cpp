//===-- test_registry_type.cpp ----------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "catch.hpp"
#include "compiler/ast/registry/registry_type.h"
#include "compiler/lexer/token_type.h"
#include <llvm/IR/Type.h>

using namespace std;

TEST_CASE("RegistryType - Enregistryr et get multiples types", "[RegistryType]") {
    RegistryType registry;

    llvm::Type* type1 = reinterpret_cast<llvm::Type*>(0x1000);
    llvm::Type* type2 = reinterpret_cast<llvm::Type*>(0x2000);
    llvm::Type* type3 = reinterpret_cast<llvm::Type*>(0x3000);

    registry.registerElement(TOKEN_IDENTIFIER, type1);
    registry.registerElement(TOKEN_LIT_INT, type2);
    registry.registerElement(TOKEN_LIT_FLOAT, type3);

    CHECK(registry.exists(TOKEN_IDENTIFIER));
    CHECK(registry.exists(TOKEN_LIT_INT));
    CHECK(registry.exists(TOKEN_LIT_FLOAT));

    CHECK(registry.get(TOKEN_IDENTIFIER) == type1);
    CHECK(registry.get(TOKEN_LIT_INT) == type2);
    CHECK(registry.get(TOKEN_LIT_FLOAT) == type3);
}

TEST_CASE("RegistryType - Type inexistant lance exception", "[RegistryType]") {
    RegistryType registry;

    CHECK_FALSE(registry.exists(TOKEN_PLUS));
    CHECK_THROWS_AS(registry.get(TOKEN_PLUS), std::invalid_argument);
}

TEST_CASE("RegistryType - Surload remplace le type existant", "[RegistryType]") {
    RegistryType registry;

    llvm::Type* type1 = reinterpret_cast<llvm::Type*>(0x1000);
    llvm::Type* type2 = reinterpret_cast<llvm::Type*>(0x2000);

    registry.registerElement(TOKEN_IDENTIFIER, type1);
    CHECK(registry.get(TOKEN_IDENTIFIER) == type1);

    registry.registerElement(TOKEN_IDENTIFIER, type2);
    CHECK(registry.get(TOKEN_IDENTIFIER) == type2);
}

TEST_CASE("RegistryType - Obtenir toutes les cles", "[RegistryType]") {
    RegistryType registry;

    registry.registerElement(TOKEN_IDENTIFIER, reinterpret_cast<llvm::Type*>(0x1000));
    registry.registerElement(TOKEN_LIT_INT, reinterpret_cast<llvm::Type*>(0x2000));
    registry.registerElement(TOKEN_LIT_FLOAT, reinterpret_cast<llvm::Type*>(0x3000));
    registry.registerElement(TOKEN_LIT_BOOL, reinterpret_cast<llvm::Type*>(0x4000));

    auto cles = registry.getKeys();
    CHECK(cles.size() == 4);
    CHECK(cles.count(TOKEN_IDENTIFIER) == 1);
    CHECK(cles.count(TOKEN_LIT_INT) == 1);
    CHECK(cles.count(TOKEN_LIT_FLOAT) == 1);
    CHECK(cles.count(TOKEN_LIT_BOOL) == 1);
}

TEST_CASE("RegistryType - Enregistryr null et get sans exception", "[RegistryType][SadTest]") {
    RegistryType registry;

    llvm::Type* typeValid = reinterpret_cast<llvm::Type*>(0x2000);

    registry.registerElement(TOKEN_IDENTIFIER, nullptr);
    registry.registerElement(TOKEN_LIT_INT, typeValid);
    registry.registerElement(TOKEN_LIT_FLOAT, nullptr);
    registry.registerElement(TOKEN_LIT_BOOL, reinterpret_cast<llvm::Type*>(0x4000));

    CHECK(registry.exists(TOKEN_IDENTIFIER));
    CHECK(registry.exists(TOKEN_LIT_INT));
    CHECK(registry.exists(TOKEN_LIT_FLOAT));
    CHECK(registry.exists(TOKEN_LIT_BOOL));

    CHECK(registry.get(TOKEN_IDENTIFIER) == nullptr);
    CHECK(registry.get(TOKEN_LIT_INT) == typeValid);
    CHECK(registry.get(TOKEN_LIT_FLOAT) == nullptr);
    CHECK(registry.get(TOKEN_LIT_BOOL) == reinterpret_cast<llvm::Type*>(0x4000));

    CHECK_THROWS_AS(registry.get(TOKEN_PLUS), std::invalid_argument);
    CHECK(registry.getKeys().size() == 4);

    registry.registerElement(TOKEN_PLUS, nullptr);
    CHECK(registry.exists(TOKEN_PLUS));
    CHECK(registry.get(TOKEN_PLUS) == nullptr);
}
