#include "catch.hpp"
#include "Compiler/AST/Registry/RegistryArgument.h"

using namespace std;

TEST_CASE("RegistryArgument - Ajouter et recuperer arguments", "[RegistryArgument]") {
    RegistryArgument registry;

    llvm::Value* arg1 = reinterpret_cast<llvm::Value*>(0x1000);
    llvm::Value* arg2 = reinterpret_cast<llvm::Value*>(0x2000);
    llvm::Value* arg3 = reinterpret_cast<llvm::Value*>(0x3000);

    registry.ajouter(arg1);
    registry.ajouter(arg2);
    registry.ajouter(arg3);

    auto& args = registry.recuperer();
    CHECK(args.size() == 3);
    CHECK(args[0] == arg1);
    CHECK(args[1] == arg2);
    CHECK(args[2] == arg3);
}

TEST_CASE("RegistryArgument - Vider et ajouter de nouveau", "[RegistryArgument]") {
    RegistryArgument registry;

    llvm::Value* arg1 = reinterpret_cast<llvm::Value*>(0x1000);
    llvm::Value* arg2 = reinterpret_cast<llvm::Value*>(0x2000);

    registry.ajouter(arg1);
    registry.ajouter(arg2);
    CHECK(registry.recuperer().size() == 2);

    registry.vider();
    CHECK(registry.recuperer().size() == 0);

    registry.ajouter(arg1);
    CHECK(registry.recuperer().size() == 1);
    CHECK(registry.recuperer()[0] == arg1);
}

TEST_CASE("RegistryArgument - Vide au demarrage", "[RegistryArgument]") {
    RegistryArgument registry;

    auto& args = registry.recuperer();
    CHECK(args.empty());
    CHECK(args.size() == 0);
}

TEST_CASE("RegistryArgument - Ordre preservation des arguments", "[RegistryArgument]") {
    RegistryArgument registry;

    llvm::Value* args[] = {
        reinterpret_cast<llvm::Value*>(0xAAAA),
        reinterpret_cast<llvm::Value*>(0xBBBB),
        reinterpret_cast<llvm::Value*>(0xCCCC),
        reinterpret_cast<llvm::Value*>(0xDDDD),
        reinterpret_cast<llvm::Value*>(0xEEEE)
    };

    for (auto arg : args) {
        registry.ajouter(arg);
    }

    auto& recuperes = registry.recuperer();
    REQUIRE(recuperes.size() == 5);

    for (size_t i = 0; i < 5; ++i) {
        CHECK(recuperes[i] == args[i]);
    }
}

TEST_CASE("RegistryArgument - Ajouter null et recuperer sans planter", "[RegistryArgument][SadTest]") {
    RegistryArgument registry;

    registry.ajouter(nullptr);
    registry.ajouter(reinterpret_cast<llvm::Value*>(0x1000));
    registry.ajouter(nullptr);

    auto& args = registry.recuperer();
    CHECK(args.size() == 3);
    CHECK(args[0] == nullptr);
    CHECK(args[1] == reinterpret_cast<llvm::Value*>(0x1000));
    CHECK(args[2] == nullptr);

    registry.vider();
    CHECK(registry.recuperer().size() == 0);

    registry.ajouter(nullptr);
    CHECK(registry.recuperer().size() == 1);
}
