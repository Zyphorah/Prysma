#include "catch.hpp"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

// Tests cas limites - Registre class

TEST_CASE("Recuperation d'une classe inexistante lance exception", "[RegistreClass]") {
    RegistreClass registre;
    CHECK_THROWS_AS(registre.recuperer("ClasseFantome"), std::invalid_argument);
}

TEST_CASE("Enregistrement et recuperation d'une classe valide", "[RegistreClass]") {
    RegistreClass registre;
    auto maClasse = make_unique<Class>();
    
    // On met quelques proprietes dans 'maClasse' pour s'assurer que c'est la meme
    maClasse->setStructType(reinterpret_cast<llvm::StructType*>(0x800));
    registre.enregistrer("MaClasseTest", std::move(maClasse));

    CHECK(registre.existe("MaClasseTest") == true);
    auto& recupererClasse = registre.recuperer("MaClasseTest");
    CHECK(recupererClasse->getStructType() == reinterpret_cast<llvm::StructType*>(0x800));
}

TEST_CASE("Ecrasement d'une classe existante avec la meme cle", "[RegistreClass]") {
    RegistreClass registre;
    
    auto classe1 = make_unique<Class>();
    classe1->setStructType(reinterpret_cast<llvm::StructType*>(0x111));
    registre.enregistrer("ClasseDoublon", std::move(classe1));
    
    auto classe2 = make_unique<Class>();
    classe2->setStructType(reinterpret_cast<llvm::StructType*>(0x222));
    registre.enregistrer("ClasseDoublon", std::move(classe2));

    auto& recuperation = registre.recuperer("ClasseDoublon");
    CHECK(recuperation->getStructType() == reinterpret_cast<llvm::StructType*>(0x222));
}

TEST_CASE("Generer un message d'erreur personnalise avec callback", "[RegistreClass]") {
    RegistreClass registre;
    registre.setMessageErreur([](const std::string& cle) {
        return "Erreur specifique : impossible de trouver " + cle;
    });

    try {
        registre.recuperer("Introuvable");
        FAIL("Devrait lancer une exception");
    } catch (const std::invalid_argument& e) {
        CHECK(string(e.what()) == "Erreur specifique : impossible de trouver Introuvable");
    }
}

TEST_CASE("Recuperation correcte des cles du registre", "[RegistreClass]") {
    RegistreClass registre;
    
    registre.enregistrer("ClasseA", make_unique<Class>());
    registre.enregistrer("ClasseB", make_unique<Class>());
    registre.enregistrer("ClasseC", make_unique<Class>());

    auto cles = registre.obtenirCles();
    CHECK(cles.size() == 3);
    CHECK(cles.count("ClasseA") == 1);
    CHECK(cles.count("ClasseB") == 1);
    CHECK(cles.count("ClasseC") == 1);
    CHECK(cles.count("ClasseInconnue") == 0);
}
