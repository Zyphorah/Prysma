#include "catch.hpp"
#include "Compiler/AST/Registry/RegistryClass.h"
#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

// Tests cas limites - Registry class

TEST_CASE("Recuperation d'une classe inexistante lance exception", "[RegistryClass]") {
    RegistryClass registry;
    CHECK_THROWS_AS(registry.recuperer("ClasseFantome"), std::invalid_argument);
}

TEST_CASE("Enregistryment et recuperation d'une classe valide", "[RegistryClass]") {
    RegistryClass registry;
    auto maClasse = make_unique<Class>();
    
    // On met quelques proprietes dans 'maClasse' pour s'assurer que c'est la meme
    maClasse->setStructType(reinterpret_cast<llvm::StructType*>(0x800));
    registry.enregistryr("MaClasseTest", std::move(maClasse));

    CHECK(registry.existe("MaClasseTest") == true);
    auto& recupererClasse = registry.recuperer("MaClasseTest");
    CHECK(recupererClasse->getStructType() == reinterpret_cast<llvm::StructType*>(0x800));
}

TEST_CASE("Ecrasement d'une classe existante avec la meme cle", "[RegistryClass]") {
    RegistryClass registry;
    
    auto classe1 = make_unique<Class>();
    classe1->setStructType(reinterpret_cast<llvm::StructType*>(0x111));
    registry.enregistryr("ClasseDoublon", std::move(classe1));
    
    auto classe2 = make_unique<Class>();
    classe2->setStructType(reinterpret_cast<llvm::StructType*>(0x222));
    registry.enregistryr("ClasseDoublon", std::move(classe2));

    auto& recuperation = registry.recuperer("ClasseDoublon");
    CHECK(recuperation->getStructType() == reinterpret_cast<llvm::StructType*>(0x222));
}

TEST_CASE("Generer un message d'error personnalise avec callback", "[RegistryClass]") {
    RegistryClass registry;
    registry.setMessageError([](const std::string& cle) {
        return "Error specifique : impossible de trouver " + cle;
    });

    try {
        registry.recuperer("Introuvable");
        FAIL("Devrait lancer une exception");
    } catch (const std::invalid_argument& e) {
        CHECK(string(e.what()) == "Error specifique : impossible de trouver Introuvable");
    }
}

TEST_CASE("Recuperation correcte des cles du registry", "[RegistryClass]") {
    RegistryClass registry;
    
    registry.enregistryr("ClasseA", make_unique<Class>());
    registry.enregistryr("ClasseB", make_unique<Class>());
    registry.enregistryr("ClasseC", make_unique<Class>());

    auto cles = registry.obtenirCles();
    CHECK(cles.size() == 3);
    CHECK(cles.count("ClasseA") == 1);
    CHECK(cles.count("ClasseB") == 1);
    CHECK(cles.count("ClasseC") == 1);
    CHECK(cles.count("ClasseInconnue") == 0);
}
