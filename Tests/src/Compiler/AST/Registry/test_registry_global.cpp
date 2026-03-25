#include "catch.hpp"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include <string>
#include <memory>

using namespace std;

TEST_CASE("Registry global vide existe returnne false", "[RegistryGlobal]") {
    RegistryFunctionGlobale registry;

    CHECK_FALSE(registry.existe("nimportequoi"));
}

TEST_CASE("Registry global vide recuperer lance exception", "[RegistryGlobal]") {
    RegistryFunctionGlobale registry;

    CHECK_THROWS_AS(registry.recuperer("absent"), std::invalid_argument);
}

TEST_CASE("Registry global ecraser ne change pas le nombre de cles", "[RegistryGlobal]") {
    RegistryFunctionGlobale registry;

    registry.enregistryr("fn", make_unique<SymboleFunctionGlobale>());
    registry.enregistryr("fn", make_unique<SymboleFunctionGlobale>());
    registry.enregistryr("fn", make_unique<SymboleFunctionGlobale>());

    auto cles = registry.obtenirCles();
    CHECK(cles.size() == 1);
}

TEST_CASE("Registry global cles similaires sont distinctes", "[RegistryGlobal]") {
    RegistryFunctionGlobale registry;

    registry.enregistryr("fn", make_unique<SymboleFunctionGlobale>());
    registry.enregistryr("Fn", make_unique<SymboleFunctionGlobale>());
    registry.enregistryr("FN", make_unique<SymboleFunctionGlobale>());

    CHECK(registry.obtenirCles().size() == 3);
    CHECK(registry.existe("fn"));
    CHECK(registry.existe("Fn"));
    CHECK(registry.existe("FN"));
}

TEST_CASE("Registry global message error contient la cle", "[RegistryGlobal]") {
    RegistryFunctionGlobale registry;

    try {
        registry.recuperer("fantome");
        FAIL("Exception attendue");
    } catch (const std::invalid_argument& e) {
        string msg = e.what();
        CHECK(msg.find("fantome") != string::npos);
    }
}

//  cas non functionnel sad test pour verifier que le registry global ne plante pas et termine correctement avec des acces concurrents a la meme cle

TEST_CASE("RegistryGlobal - Acces concurrent a la meme cle ne plante pas", "[RegistryGlobal][SadTest]") {
    RegistryFunctionGlobale registry;

    registry.enregistryr("fn", make_unique<SymboleFunctionGlobale>());

    CHECK_NOTHROW(registry.recuperer("fn"));
    CHECK_NOTHROW(registry.recuperer("fn"));
    CHECK_NOTHROW(registry.recuperer("fn"));

    registry.enregistryr("fn", make_unique<SymboleFunctionGlobale>());
    CHECK_NOTHROW(registry.recuperer("fn"));

    CHECK(registry.existe("fn"));
    auto cles = registry.obtenirCles();
    CHECK(cles.size() == 1);
    
    CHECK_THROWS_AS(registry.recuperer("cle_invalide"), std::invalid_argument);
}
