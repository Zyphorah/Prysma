#include "catch.hpp"
#include "Compilateur/AST/Registre/RegistreArgument.h"

using namespace std;

TEST_CASE("RegistreArgument - Ajouter et recuperer arguments", "[RegistreArgument]") {
    RegistreArgument registre;

    llvm::Value* arg1 = reinterpret_cast<llvm::Value*>(0x1000);
    llvm::Value* arg2 = reinterpret_cast<llvm::Value*>(0x2000);
    llvm::Value* arg3 = reinterpret_cast<llvm::Value*>(0x3000);

    registre.ajouter(arg1);
    registre.ajouter(arg2);
    registre.ajouter(arg3);

    auto& args = registre.recuperer();
    CHECK(args.size() == 3);
    CHECK(args[0] == arg1);
    CHECK(args[1] == arg2);
    CHECK(args[2] == arg3);
}

TEST_CASE("RegistreArgument - Vider et ajouter de nouveau", "[RegistreArgument]") {
    RegistreArgument registre;

    llvm::Value* arg1 = reinterpret_cast<llvm::Value*>(0x1000);
    llvm::Value* arg2 = reinterpret_cast<llvm::Value*>(0x2000);

    registre.ajouter(arg1);
    registre.ajouter(arg2);
    CHECK(registre.recuperer().size() == 2);

    registre.vider();
    CHECK(registre.recuperer().size() == 0);

    registre.ajouter(arg1);
    CHECK(registre.recuperer().size() == 1);
    CHECK(registre.recuperer()[0] == arg1);
}

TEST_CASE("RegistreArgument - Vide au demarrage", "[RegistreArgument]") {
    RegistreArgument registre;

    auto& args = registre.recuperer();
    CHECK(args.empty());
    CHECK(args.size() == 0);
}

TEST_CASE("RegistreArgument - Ordre preservation des arguments", "[RegistreArgument]") {
    RegistreArgument registre;

    llvm::Value* args[] = {
        reinterpret_cast<llvm::Value*>(0xAAAA),
        reinterpret_cast<llvm::Value*>(0xBBBB),
        reinterpret_cast<llvm::Value*>(0xCCCC),
        reinterpret_cast<llvm::Value*>(0xDDDD),
        reinterpret_cast<llvm::Value*>(0xEEEE)
    };

    for (auto arg : args) {
        registre.ajouter(arg);
    }

    auto& recuperes = registre.recuperer();
    REQUIRE(recuperes.size() == 5);

    for (size_t i = 0; i < 5; ++i) {
        CHECK(recuperes[i] == args[i]);
    }
}

TEST_CASE("RegistreArgument - Ajouter null et recuperer sans planter", "[RegistreArgument][SadTest]") {
    RegistreArgument registre;

    registre.ajouter(nullptr);
    registre.ajouter(reinterpret_cast<llvm::Value*>(0x1000));
    registre.ajouter(nullptr);

    auto& args = registre.recuperer();
    CHECK(args.size() == 3);
    CHECK(args[0] == nullptr);
    CHECK(args[1] == reinterpret_cast<llvm::Value*>(0x1000));
    CHECK(args[2] == nullptr);

    registre.vider();
    CHECK(registre.recuperer().size() == 0);

    registre.ajouter(nullptr);
    CHECK(registre.recuperer().size() == 1);
}
