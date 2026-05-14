//===-- main.cpp ------------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/utils/orchestrator_include/orchestrator_include.h"
#include "compiler/file_processing/builder_systeme.h"
#include "compiler/file_processing/file_reading.h"
#include "compiler/manager_error.h"
#include "compiler/registry/registry_file.h"
#include <exception>
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <filesystem>
#include <cstdlib>
#include <mutex>
#include <llvm/Support/TargetSelect.h>
#include <string>
#include <vector>


auto main(int argc, char* argv[]) -> int
{
    if (argc < 2) {
        std::cerr << "Error: Aucun fichier spécifié" << '\n';
        std::cerr << "Usage: Prysma <fichier.p> [--graphviz]" << '\n';
        return 1;
    }
    
    std::string cheminFile;
    bool activerGraphViz = false;

    std::vector<std::string> arguments;
    for (int i = 1; i < argc; ++i) {
        arguments.emplace_back(argv[i]); // NOLINT
    }
    for (const auto& arg : arguments) {
        if (arg == "--graphviz") {
            activerGraphViz = true;
        } else {
            cheminFile = arg;
        }
    }

    if (cheminFile.empty()) {
        std::cerr << "Error: Aucun fichier spécifié" << '\n';
        return 1;
    }

    std::string nomFile = std::filesystem::path(cheminFile).filename().string(); 
    try {  
        
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        
        std::mutex mutex;
      
        std::filesystem::path exePath = std::filesystem::canonical("/proc/self/exe");
        std::filesystem::path buildDir = exePath.parent_path();
        std::filesystem::path racineProject = buildDir.parent_path();

        // ./Prysma --graphviz fichier.p pour activer la génération du graphe AST
        std::filesystem::create_directories(buildDir / "programme");
        if (activerGraphViz) {
            std::filesystem::create_directories(buildDir / "graphe");
        }

        std::string srcLib = (racineProject / "src" / "lib").string();
        std::string objLib = (buildDir / "obj" / "lib").string();

        RegistryFunctionGlobal registryFunctionGlobale;
        RegistryClassGlobal registryClassGlobale;

        FileRegistry registryFiles;
        
        OrchestratorInclude orchestratorInclude(
            registryFunctionGlobale, 
            registryClassGlobale, 
            registryFiles, 
            mutex, 
            activerGraphViz);
            orchestratorInclude.compileProject(cheminFile);
        if (orchestratorInclude.hasErrors()) {
            std::cerr << "Error: La compilation a échoué, arrêt du processus." << '\n';
            return 1;
        }
        std::string nomExecutable = std::filesystem::path(cheminFile).stem().string();

        BuilderSystem::BuilderParams params = {
            srcLib,
            objLib,
            buildDir.string(),
            registryFiles.getAllFiles(),
            nomExecutable
        };
        BuilderSystem builder(params);
        builder.compileLib();
        builder.linkLibExecutable();
    }
    catch (const CompilationError& error) {
        std::cerr << nomFile << ":" 
                  << error.getLine() << ":" 
                  << error.getColumn() << ": Error: " 
                  << error.what() << '\n';
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << cheminFile << ":1:1: Error: " << e.what() << '\n';
        return 1;
    }
}
