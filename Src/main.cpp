#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Utils/OrchestratorInclude/ConfigurationFacadeEnvironment.h"
#include "Compiler/AST/Utils/OrchestratorInclude/OrchestratorInclude.h"
#include "Compiler/FileProcessing/BuilderSysteme.h"
#include "Compiler/FileProcessing/FileReading.h"
#include "Compiler/ManagerError.h"
#include "Compiler/Registry/RegistryFile.h"
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
// Actuellement le compiler utilise des nodes contenant des objects de type std::string et std::vector pour stocker les informations de l'AST
// Le problème est que les types std::string créer un object dynamiquement avec un new caché 
// et que les std::vector font la même chose. La destruction des objects std::string et std::vector n'est pas faite par 
// le bump allocator, nous devons faire un delete manuel pour supprimer chaque object de façon récursive dans L'AST, ça annule les avantages du bump allocator 
// et peut causer des problèmes de performances et de fragmentation de mémoire. La solution est d'utiliser des llvm::StringRef et des llvm::ArrayRef qui sont des vues 
// sur des données contigues en mémoire et qui ne font pas d'allocation dynamique, cela permet de supprimer complètement les std::string et std::vector et permettre de 
// retirer les fuites de mémoire causé par l'allocation dynamique des std::string et std::vector. 

// Ne pas oblier de faire le système de delete en onion pour les objects de la classe du langage prysma. 

// Actuellement, les méthodes sont callées de façon statique seulement (object.methode()) et il n'y a pas de système pour caller une méthode sur un object returnné par un autre object.
// Exemple : object.methode().methode() ; l'call d'une function en chaîne est impossible.

// J'utilise actuellement la syntaxe "dec ptr object = new Classe();". Le problème est que l'object est opaque, on ne peut pas déterminer son type. Par exemple, si une classe contient un object et que je fais un get pour faire un call de function dessus, je ne peux pas déterminer le type de l'object pour effectuer ce call.
// Avec "dec ptr object2 = object.getA();", qui fait un get d'un object d'un autre type, ce type est inconnu au moment d'caller la méthode.
// Dans le registry actuel, comme la function n'existe pas pour l'object opaque, le call est impossible. Pour résoudre ce problème, la conception du système de typage dois être revue. 

// Prouver mathématiquement que mon système est infaillible pour la compilation comme Coq, Isabelle/HOL ou TLA+, reécrire le compiler dans un langage mathématique
// Engine de Réflexion LLVM génération automatique du code llvm 

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

        std::string srcLib = (racineProject / "Src" / "Lib").string();
        std::string objLib = (buildDir / "obj" / "Lib").string();

        std::unique_ptr<RegistryFunctionGlobale> registryFunctionGlobale = std::make_unique<RegistryFunctionGlobale>();
        std::unique_ptr<RegistryFile> registryFiles = std::make_unique<RegistryFile>();
        std::unique_ptr<ConfigurationFacadeEnvironnement> facadeConfigurationEnvironnement = std::make_unique<ConfigurationFacadeEnvironnement>(registryFunctionGlobale.get(), registryFiles.get());
        
        OrchestratorInclude orchestratorInclude(registryFunctionGlobale.get(), registryFiles.get(), mutex.get(), activerGraphViz);
        orchestratorInclude.compilerProject(cheminFile);

        if (orchestratorInclude.aDesErrors()) {
            std::cerr << "Error: La compilation a échoué, arrêt du processus." << std::endl;
            return 1;
        }

        std::string nomExecutable = std::filesystem::path(cheminFile).stem().string();

        BuilderSysteme::BuilderParams params = {
            srcLib,
            objLib,
            buildDir.string(),
            registryFiles->obtenirTousLesFiles(),
            nomExecutable
        };
        BuilderSysteme builder(params);
        builder.compilerLib();
        builder.lierLibExecutable();
    }
    catch (const ErrorCompilation& error) {
        std::cerr << nomFile << ":" 
                  << error.getLigne() << ":" 
                  << error.getColonne() << ": Error: " 
                  << error.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}