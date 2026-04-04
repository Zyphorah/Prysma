//===-- test_registry_argument.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "catch.hpp"
#include "compiler/ast/registry/registry_argument.h"

using namespace std;

TEST_CASE("RegistryArgument - Ajouter et get arguments", "[RegistryArgument]") {
    RegistryArgument registry;

    llvm::Value* arg1 = reinterpret_cast<llvm::Value*>(0x1000);
    llvm::Value* arg2 = reinterpret_cast<llvm::Value*>(0x2000);
    llvm::Value* arg3 = reinterpret_cast<llvm::Value*>(0x3000);

    registry.add(arg1);
    registry.add(arg2);
    registry.add(arg3);

    auto& args = registry.get();
    CHECK(args.size() == 3);
    CHECK(args[0] == arg1);
    CHECK(args[1] == arg2);
    CHECK(args[2] == arg3);
}

TEST_CASE("RegistryArgument - Vider et add de nouveau", "[RegistryArgument]") {
    RegistryArgument registry;

    llvm::Value* arg1 = reinterpret_cast<llvm::Value*>(0x1000);
    llvm::Value* arg2 = reinterpret_cast<llvm::Value*>(0x2000);

    registry.add(arg1);
    registry.add(arg2);
    CHECK(registry.get().size() == 2);

    registry.clear();
    CHECK(registry.get().size() == 0);

    registry.add(arg1);
    CHECK(registry.get().size() == 1);
    CHECK(registry.get()[0] == arg1);
}

TEST_CASE("RegistryArgument - Vide au demarrage", "[RegistryArgument]") {
    RegistryArgument registry;

    auto& args = registry.get();
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
        registry.add(arg);
    }

    auto& recuperes = registry.get();
    REQUIRE(recuperes.size() == 5);

    for (size_t i = 0; i < 5; ++i) {
        CHECK(recuperes[i] == args[i]);
    }
}

TEST_CASE("RegistryArgument - Ajouter null et get sans planter", "[RegistryArgument][SadTest]") {
    RegistryArgument registry;

    registry.add(nullptr);
    registry.add(reinterpret_cast<llvm::Value*>(0x1000));
    registry.add(nullptr);

    auto& args = registry.get();
    CHECK(args.size() == 3);
    CHECK(args[0] == nullptr);
    CHECK(args[1] == reinterpret_cast<llvm::Value*>(0x1000));
    CHECK(args[2] == nullptr);

    registry.clear();
    CHECK(registry.get().size() == 0);

    registry.add(nullptr);
    CHECK(registry.get().size() == 1);
}
