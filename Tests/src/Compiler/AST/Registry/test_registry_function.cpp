#include "catch.hpp"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include <string>
#include <memory>

using namespace std;

// Tests du registry de functions (globale et locale)

TEST_CASE("Enregistryr et recuperer une function globale", "[RegistryFunction]") {
    RegistryFunctionGlobale registry;

    auto symbole = make_unique<SymboleFunctionGlobale>();
    symbole->typeReturn = nullptr;
    symbole->node = nullptr;

    registry.enregistryr("maFunction", std::move(symbole));

    CHECK(registry.existe("maFunction"));

    const auto& recupere = registry.recuperer("maFunction");
    auto* symboleCast = dynamic_cast<SymboleFunctionGlobale*>(recupere.get());
    REQUIRE(symboleCast != nullptr);
}

TEST_CASE("Recuperer une function inexistante lance une exception", "[RegistryFunction]") {
    RegistryFunctionGlobale registry;

    CHECK_FALSE(registry.existe("fantome"));
    CHECK_THROWS_AS(registry.recuperer("fantome"), std::invalid_argument);
}

TEST_CASE("Enregistryr plusieurs functions et verifier les cles", "[RegistryFunction]") {
    RegistryFunctionGlobale registry;

    registry.enregistryr("fn_a", make_unique<SymboleFunctionGlobale>());
    registry.enregistryr("fn_b", make_unique<SymboleFunctionGlobale>());
    registry.enregistryr("fn_c", make_unique<SymboleFunctionGlobale>());

    auto cles = registry.obtenirCles();
    CHECK(cles.size() == 3);
    CHECK(cles.count("fn_a") == 1);
    CHECK(cles.count("fn_b") == 1);
    CHECK(cles.count("fn_c") == 1);
}

TEST_CASE("Ecraser une function existante dans le registry", "[RegistryFunction]") {
    RegistryFunctionLocale registry;

    auto premier = make_unique<SymboleFunctionLocale>();
    premier->function = nullptr;
    registry.enregistryr("update", std::move(premier));

    auto second = make_unique<SymboleFunctionLocale>();
    second->function = reinterpret_cast<llvm::Function*>(0xDEAD);
    registry.enregistryr("update", std::move(second));

    const auto& recupere = registry.recuperer("update");
    auto* symboleCast = dynamic_cast<SymboleFunctionLocale*>(recupere.get());
    REQUIRE(symboleCast != nullptr);
    CHECK(symboleCast->function == reinterpret_cast<llvm::Function*>(0xDEAD));
}

TEST_CASE("Registry local functionne independamment du global", "[RegistryFunction]") {
    RegistryFunctionGlobale registryGlobal;
    RegistryFunctionLocale registryLocal;

    registryGlobal.enregistryr("shared", make_unique<SymboleFunctionGlobale>());
    registryLocal.enregistryr("shared", make_unique<SymboleFunctionLocale>());

    CHECK(registryGlobal.existe("shared"));
    CHECK(registryLocal.existe("shared"));

    const auto& globale = registryGlobal.recuperer("shared");
    const auto& locale = registryLocal.recuperer("shared");

    CHECK(dynamic_cast<SymboleFunctionGlobale*>(globale.get()) != nullptr);
    CHECK(dynamic_cast<SymboleFunctionLocale*>(locale.get()) != nullptr);
}

// cas non functionnel sad test 
TEST_CASE("RegistryFunction - Enregistryr null et recuperer ne plante pas", "[RegistryFunction][SadTest]") {
    RegistryFunctionLocale registry;

    registry.enregistryr("fn1", make_unique<SymboleFunctionLocale>());
    registry.enregistryr("fn2", make_unique<SymboleFunctionLocale>());

    CHECK(registry.existe("fn1"));
    CHECK(registry.existe("fn2"));
    CHECK(registry.obtenirCles().size() == 2);

    auto fnUpdate = make_unique<SymboleFunctionLocale>();
    fnUpdate->function = reinterpret_cast<llvm::Function*>(0xABCD);
    registry.enregistryr("fn1", std::move(fnUpdate));

    const auto& recupere = registry.recuperer("fn1");
    auto* symbole = dynamic_cast<SymboleFunctionLocale*>(recupere.get());
    REQUIRE(symbole != nullptr);
    CHECK(symbole->function == reinterpret_cast<llvm::Function*>(0xABCD));

    CHECK(registry.obtenirCles().size() == 2);
    CHECK_THROWS_AS(registry.recuperer("fn_inexistante"), std::invalid_argument);
}
