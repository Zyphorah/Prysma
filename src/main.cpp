//===-- main.cpp - Brief description of the file -------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//
#include "compiler/ast/utils/orchestrator_include/configuration_facade_environment.h"
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
#include <memory>
#include <mutex>
#include <llvm/Support/TargetSelect.h>
#include <string>
#include <vector>


// Polyspace analyse static prouvé mathématiquement que l'execution d'une function ne cause pas de null pointer ou certaine catégorie de crash

// Ne pas oblier de faire le système de delete en onion pour les objects de la classe du langage prysma. 

// Actuellement, les méthodes sont callées de façon statique seulement (object.methode()) et il n'y a pas de système pour caller une méthode sur un object returnné par un autre object.
// Exemple : object.methode().methode() ; l'call d'une function en chaîne est impossible.

// J'utilise actuellement la syntaxe "dec ptr object = new Classe();". Le problème est que l'object est opaque, on ne peut pas déterminer son type. Par exemple, si une classe contient un object et que je fais un get pour faire un call de function dessus, je ne peux pas déterminer le type de l'object pour effectuer ce call.
// Avec "dec ptr object2 = object.getA();", qui fait un get d'un object d'un autre type, ce type est inconnu au moment d'caller la méthode.
// Dans le registry actuel, comme la function n'existe pas pour l'object opaque, le call est impossible. Pour résoudre ce problème, la conception du système de typage dois être revue. 

// Prouver mathématiquement que mon système est infaillible pour la compilation comme Coq, Isabelle/HOL ou TLA+, reécrire le compiler dans un langage mathématique
// Engine de Réflexion LLVM génération automatique du code llvm 
// Pool string, registre afin d'éviter la dupplication et permettre la comparaison par pointeur. 

// Améliorations possibles performance : 
// DFA pour le lexer permet de gagner 10 à 20 fois supérieur en vitesse de tokenisation par rapport à l'approche de base 
// Pour un gain de 20 pourcents en vitesse 
// Faire un système de pool string pour le système complet au lieu d'utiliser les strings lente de std::string qui ne sont pas contigue en mémoire et qui peuvent causer des problèmes de performance et de fragmentation mémoire.
// Utilisation du polymorphisme statique avec des templates pour la vitesse d'exécution
// Utilisation d'un système de table compile time au lieu des registrys dynamiques c'est plus rapide 10 20 pourcent de gain en vitesse d'exécution
// Swiss Tables pour les tables de hachage compile time
// Utilisation d'un DFA mais avec SIMD (Single Instruction, Multiple Data), pour un maximum de gain de performance
// Utilisation d'un système multi thread pour la compilation, un thread par fichier source. 
// Transformer la structure de l'tree syntaxique abstrait en "SoA" (Structure of Arrays) amélioration de la localité du cache et de la performance d'exécution. 
// La Sharded Map pour les registry de function afin de retirer le golo d'étranglement du mutex pour les registrys de function, chaque thread aura sa propre 
// shard de la map pour les functions. Pour un gain d'environ 70 pourcents en vitesse d'exécution pour les project avec beaucoup de function.

// FUTAMURA projection pour faire un compiler qui se compile lui même

// Calculer le nombre d'octet d'un fichier pour déterminer la taille d'un vecteur en mémoire évite la copy lors qu'il grandi. 
// MemoryBuffer lecture ultra rapide des fichiers permet de faire corespondre directement le binaire. 

// KCachegrind pour déterminer les performances 
// Recyclage des noeuds de l'AST pour éviter les allocations sur des noeuds existants, pas très bien pour les performances

auto main(int argc, char* argv[]) -> int
{
    if (argc < 2) {
        std::cerr << "Error: Aucun fichier spécifié" << std::endl;
        std::cerr << "Usage: Prysma <fichier.p> [--graphviz]" << std::endl;
        return 1;
    }
    
    std::string cheminFile;
    bool activerGraphViz = false;

    std::vector<std::string> arguments(argv + 1, argv + argc);
    for (const auto& arg : arguments) {
        if (arg == "--graphviz") {
            activerGraphViz = true;
        } else {
            cheminFile = arg;
        }
    }

    if (cheminFile.empty()) {
        std::cerr << "Error: Aucun fichier spécifié" << std::endl;
        return 1;
    }

    std::string nomFile = std::filesystem::path(cheminFile).filename().string(); 
    try {  
        
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        
        std::unique_ptr<std::mutex> mutex = std::make_unique<std::mutex>();
      
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

        std::unique_ptr<RegistryFunctionGlobal> registryFunctionGlobale = std::make_unique<RegistryFunctionGlobal>();
        std::unique_ptr<FileRegistry> registryFiles = std::make_unique<FileRegistry>();
        std::unique_ptr<ConfigurationFacadeEnvironment> facadeConfigurationEnvironnement = std::make_unique<ConfigurationFacadeEnvironment>(registryFunctionGlobale.get(), registryFiles.get());
        
        OrchestratorInclude orchestratorInclude(registryFunctionGlobale.get(), registryFiles.get(), mutex.get(), activerGraphViz);
        orchestratorInclude.compileProject(cheminFile);

        if (orchestratorInclude.hasErrors()) {
            std::cerr << "Error: La compilation a échoué, arrêt du processus." << std::endl;
            return 1;
        }

        std::string nomExecutable = std::filesystem::path(cheminFile).stem().string();

        BuilderSystem::BuilderParams params = {
            srcLib,
            objLib,
            buildDir.string(),
            registryFiles->getAllFiles(),
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
                  << error.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
