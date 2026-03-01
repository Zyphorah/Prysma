#include "catch.hpp"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include <string>
#include <memory>

using namespace std;

// Tests du registre de fonctions (globale et locale)

TEST_CASE("Enregistrer et recuperer une fonction globale", "[RegistreFonction]") {
    RegistreFonctionGlobale registre;

    auto symbole = make_unique<SymboleFonctionGlobale>();
    symbole->typeRetour = nullptr;
    symbole->noeud = nullptr;

    registre.enregistrer("maFonction", std::move(symbole));

    CHECK(registre.existe("maFonction"));

    const auto& recupere = registre.recuperer("maFonction");
    auto* symboleCast = dynamic_cast<SymboleFonctionGlobale*>(recupere.get());
    REQUIRE(symboleCast != nullptr);
}

TEST_CASE("Recuperer une fonction inexistante lance une exception", "[RegistreFonction]") {
    RegistreFonctionGlobale registre;

    CHECK_FALSE(registre.existe("fantome"));
    CHECK_THROWS_AS(registre.recuperer("fantome"), std::invalid_argument);
}

TEST_CASE("Enregistrer plusieurs fonctions et verifier les cles", "[RegistreFonction]") {
    RegistreFonctionGlobale registre;

    registre.enregistrer("fn_a", make_unique<SymboleFonctionGlobale>());
    registre.enregistrer("fn_b", make_unique<SymboleFonctionGlobale>());
    registre.enregistrer("fn_c", make_unique<SymboleFonctionGlobale>());

    auto cles = registre.obtenirCles();
    CHECK(cles.size() == 3);
    CHECK(cles.count("fn_a") == 1);
    CHECK(cles.count("fn_b") == 1);
    CHECK(cles.count("fn_c") == 1);
}

TEST_CASE("Ecraser une fonction existante dans le registre", "[RegistreFonction]") {
    RegistreFonctionLocale registre;

    auto premier = make_unique<SymboleFonctionLocale>();
    premier->fonction = nullptr;
    registre.enregistrer("update", std::move(premier));

    auto second = make_unique<SymboleFonctionLocale>();
    second->fonction = reinterpret_cast<llvm::Function*>(0xDEAD);
    registre.enregistrer("update", std::move(second));

    const auto& recupere = registre.recuperer("update");
    auto* symboleCast = dynamic_cast<SymboleFonctionLocale*>(recupere.get());
    REQUIRE(symboleCast != nullptr);
    CHECK(symboleCast->fonction == reinterpret_cast<llvm::Function*>(0xDEAD));
}

TEST_CASE("Registre local fonctionne independamment du global", "[RegistreFonction]") {
    RegistreFonctionGlobale registreGlobal;
    RegistreFonctionLocale registreLocal;

    registreGlobal.enregistrer("shared", make_unique<SymboleFonctionGlobale>());
    registreLocal.enregistrer("shared", make_unique<SymboleFonctionLocale>());

    CHECK(registreGlobal.existe("shared"));
    CHECK(registreLocal.existe("shared"));

    const auto& globale = registreGlobal.recuperer("shared");
    const auto& locale = registreLocal.recuperer("shared");

    CHECK(dynamic_cast<SymboleFonctionGlobale*>(globale.get()) != nullptr);
    CHECK(dynamic_cast<SymboleFonctionLocale*>(locale.get()) != nullptr);
}

// cas non fonctionnel sad test 
TEST_CASE("RegistreFonction - Enregistrer null et recuperer ne plante pas", "[RegistreFonction][SadTest]") {
    RegistreFonctionLocale registre;

    registre.enregistrer("fn1", make_unique<SymboleFonctionLocale>());
    registre.enregistrer("fn2", make_unique<SymboleFonctionLocale>());

    CHECK(registre.existe("fn1"));
    CHECK(registre.existe("fn2"));
    CHECK(registre.obtenirCles().size() == 2);

    auto fnUpdate = make_unique<SymboleFonctionLocale>();
    fnUpdate->fonction = reinterpret_cast<llvm::Function*>(0xABCD);
    registre.enregistrer("fn1", std::move(fnUpdate));

    const auto& recupere = registre.recuperer("fn1");
    auto* symbole = dynamic_cast<SymboleFonctionLocale*>(recupere.get());
    REQUIRE(symbole != nullptr);
    CHECK(symbole->fonction == reinterpret_cast<llvm::Function*>(0xABCD));

    CHECK(registre.obtenirCles().size() == 2);
    CHECK_THROWS_AS(registre.recuperer("fn_inexistante"), std::invalid_argument);
}
