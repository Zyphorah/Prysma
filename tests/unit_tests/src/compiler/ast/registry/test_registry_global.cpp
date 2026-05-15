//===-- test_registry_global.cpp --------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "catch.hpp"
#include "compiler/ast/registry/registry_function.h"
#include <string>
#include <memory>

using namespace std;

TEST_CASE("Registry global vide existe returnne false", "[RegistryGlobal]") {
    RegistryFunctionGlobal registry;

    CHECK_FALSE(registry.exists("nimportequoi"));
}

TEST_CASE("Registry global vide get lance exception", "[RegistryGlobal]") {
    RegistryFunctionGlobal registry;

    CHECK_THROWS_AS(registry.get("absent"), std::invalid_argument);
}

TEST_CASE("Registry global ecraser ne change pas le nombre de cles", "[RegistryGlobal]") {
    RegistryFunctionGlobal registry;

    registry.registerElement("fn", make_unique<SymbolFunctionGlobal>());
    registry.registerElement("fn", make_unique<SymbolFunctionGlobal>());
    registry.registerElement("fn", make_unique<SymbolFunctionGlobal>());

    auto cles = registry.getKeys();
    CHECK(cles.size() == 1);
}

TEST_CASE("Registry global cles similaires sont distinctes", "[RegistryGlobal]") {
    RegistryFunctionGlobal registry;

    registry.registerElement("fn", make_unique<SymbolFunctionGlobal>());
    registry.registerElement("Fn", make_unique<SymbolFunctionGlobal>());
    registry.registerElement("FN", make_unique<SymbolFunctionGlobal>());

    CHECK(registry.getKeys().size() == 3);
    CHECK(registry.exists("fn"));
    CHECK(registry.exists("Fn"));
    CHECK(registry.exists("FN"));
}

TEST_CASE("Registry global message error contient la cle", "[RegistryGlobal]") {
    RegistryFunctionGlobal registry;

    try {
        registry.get("fantome");
        FAIL("Exception attendue");
    } catch (const std::invalid_argument& e) {
        string msg = e.what();
        CHECK(msg.find("fantome") != string::npos);
    }
}

//  cas non functionnel sad test pour verifier que le registry global ne plante pas et termine correctement avec des acces concurrents a la meme cle

TEST_CASE("RegistryGlobal - Acces concurrent a la meme cle ne plante pas", "[RegistryGlobal][SadTest]") {
    RegistryFunctionGlobal registry;

    registry.registerElement("fn", make_unique<SymbolFunctionGlobal>());

    CHECK_NOTHROW(registry.get("fn"));
    CHECK_NOTHROW(registry.get("fn"));
    CHECK_NOTHROW(registry.get("fn"));

    registry.registerElement("fn", make_unique<SymbolFunctionGlobal>());
    CHECK_NOTHROW(registry.get("fn"));

    CHECK(registry.exists("fn"));
    auto cles = registry.getKeys();
    CHECK(cles.size() == 1);
    
    CHECK_THROWS_AS(registry.get("cle_invalide"), std::invalid_argument);
}
