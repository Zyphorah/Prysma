//===-- test_registry_class.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "catch.hpp"
#include "compiler/ast/registry/registry_class.h"
#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

// Tests cas limites - Registry class

TEST_CASE("Recuperation d'une classe inexistante lance exception", "[RegistryClass]") {
    RegistryClass registry;
    CHECK_THROWS_AS(registry.get("ClasseFantome"), std::invalid_argument);
}

TEST_CASE("Enregistryment et recuperation d'une classe valide", "[RegistryClass]") {
    RegistryClass registry;
    auto maClasse = make_unique<Class>();
    
    // On met quelques proprietes dans 'maClasse' pour s'assurer que c'est la meme
    maClasse->setStructType(reinterpret_cast<llvm::StructType*>(0x800));
    registry.registerElement("MaClasseTest", std::move(maClasse));

    CHECK(registry.exists("MaClasseTest") == true);
    auto& getClasse = registry.get("MaClasseTest");
    CHECK(getClasse->getStructType() == reinterpret_cast<llvm::StructType*>(0x800));
}

TEST_CASE("Ecrasement d'une classe existante avec la meme cle", "[RegistryClass]") {
    RegistryClass registry;
    
    auto classe1 = make_unique<Class>();
    classe1->setStructType(reinterpret_cast<llvm::StructType*>(0x111));
    registry.registerElement("ClasseDoublon", std::move(classe1));
    
    auto classe2 = make_unique<Class>();
    classe2->setStructType(reinterpret_cast<llvm::StructType*>(0x222));
    registry.registerElement("ClasseDoublon", std::move(classe2));

    auto& recuperation = registry.get("ClasseDoublon");
    CHECK(recuperation->getStructType() == reinterpret_cast<llvm::StructType*>(0x222));
}

TEST_CASE("Generer un message d'error personnalise avec callback", "[RegistryClass]") {
    RegistryClass registry;
    registry.setErrorMessage([](const std::string& cle) {
        return "Error specifique : impossible de trouver " + cle;
    });

    try {
        registry.get("Introuvable");
        FAIL("Devrait lancer une exception");
    } catch (const std::invalid_argument& e) {
        CHECK(string(e.what()) == "Error specifique : impossible de trouver Introuvable");
    }
}

TEST_CASE("Recuperation correcte des cles du registry", "[RegistryClass]") {
    RegistryClass registry;
    
    registry.registerElement("ClasseA", make_unique<Class>());
    registry.registerElement("ClasseB", make_unique<Class>());
    registry.registerElement("ClasseC", make_unique<Class>());

    auto cles = registry.getKeys();
    CHECK(cles.size() == 3);
    CHECK(cles.count("ClasseA") == 1);
    CHECK(cles.count("ClasseB") == 1);
    CHECK(cles.count("ClasseC") == 1);
    CHECK(cles.count("ClasseInconnue") == 0);
}
