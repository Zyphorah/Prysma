#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Registre/RegistreFichier.h"
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

// Actuellement le compilateur utilise des noeuds contenant des objets de type std::string et std::vector pour stocker les informations de l'AST
// Le problème est que les types std::string créer un objet dynamiquement avec un new caché 
// et que les std::vector font la même chose. La destruction des objets std::string et std::vector n'est pas faite par 
// le bump allocator, nous devons faire un delete manuel pour supprimer chaque objet de façon récursive dans L'AST, ça annule les avantages du bump allocator 
// et peut causer des problèmes de performances et de fragmentation de mémoire. La solution est d'utiliser des llvm::StringRef et des llvm::ArrayRef qui sont des vues 
// sur des données contigues en mémoire et qui ne font pas d'allocation dynamique, cela permet de supprimer complètement les std::string et std::vector et permettre de 
// retirer les fuites de mémoire causé par l'allocation dynamique des std::string et std::vector. 

// Ne pas oblier de faire le système de delete en onion pour les objets de la classe du langage prysma. 

// Actuellement, les méthodes sont appelées de façon statique seulement (objet.methode()) et il n'y a pas de système pour appeler une méthode sur un objet retourné par un autre objet.
// Exemple : objet.methode().methode() ; l'appel d'une fonction en chaîne est impossible.

// J'utilise actuellement la syntaxe "dec ptr objet = new Classe();". Le problème est que l'objet est opaque, on ne peut pas déterminer son type. Par exemple, si une classe contient un objet et que je fais un get pour faire un call de fonction dessus, je ne peux pas déterminer le type de l'objet pour effectuer ce call.
// Avec "dec ptr objet2 = objet.getA();", qui fait un get d'un objet d'un autre type, ce type est inconnu au moment d'appeler la méthode.
// Dans le registre actuel, comme la fonction n'existe pas pour l'objet opaque, le call est impossible. Pour résoudre ce problème, la conception du système de typage dois être revue. 

// Prouver mathématiquement que mon système est infaillible pour la compilation comme Coq, Isabelle/HOL ou TLA+, reécrire le compilateur dans un langage mathématique
// Moteur de Réflexion LLVM génération automatique du code llvm 

// Améliorations possibles performance : 
// DFA pour le lexer permet de gagner 10 à 20 fois supérieur en vitesse de tokenisation par rapport à l'approche de base 
// Pour un gain de 20 pourcents en vitesse 
// Faire un système de pool string pour le système complet au lieu d'utiliser les strings lente de std::string qui ne sont pas contigue en mémoire et qui peuvent causer des problèmes de performance et de fragmentation mémoire.
// Utilisation du polymorphisme statique avec des templates pour la vitesse d'exécution
// Utilisation d'un système de table compile time au lieu des registres dynamiques c'est plus rapide 10 20 pourcent de gain en vitesse d'exécution
// Swiss Tables pour les tables de hachage compile time
// Utilisation d'un DFA mais avec SIMD (Single Instruction, Multiple Data), pour un maximum de gain de performance
// Utilisation d'un système multi thread pour la compilation, un thread par fichier source. 
// Transformer la structure de l'arbre syntaxique abstrait en "SoA" (Structure of Arrays) amélioration de la localité du cache et de la performance d'exécution. 
// La Sharded Map pour les registre de fonction afin de retirer le golo d'étranglement du mutex pour les registres de fonction, chaque thread aura sa propre 
// shard de la map pour les fonctions. Pour un gain d'environ 70 pourcents en vitesse d'exécution pour les projet avec beaucoup de fonction.

// FUTAMURA projection pour faire un compilateur qui se compile lui même

auto main(int argc, char* argv[]) -> int
{
    if (argc < 2) {
        std::cerr << "Erreur: Aucun fichier spécifié" << std::endl;
        std::cerr << "Usage: Prysma <fichier.p> [--graphviz]" << std::endl;
        return 1;
    }
    
    std::string cheminFichier;
    bool activerGraphViz = false;

    std::vector<std::string> arguments(argv + 1, argv + argc);
    for (const auto& arg : arguments) {
        if (arg == "--graphviz") {
            activerGraphViz = true;
        } else {
            cheminFichier = arg;
        }
    }

    if (cheminFichier.empty()) {
        std::cerr << "Erreur: Aucun fichier spécifié" << std::endl;
        return 1;
    }

    std::string nomFichier = std::filesystem::path(cheminFichier).filename().string(); 
    try {  
        
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        
        std::unique_ptr<std::mutex> mutex = std::make_unique<std::mutex>();
      
        std::filesystem::path exePath = std::filesystem::canonical("/proc/self/exe");
        std::filesystem::path buildDir = exePath.parent_path();
        std::filesystem::path racineProjet = buildDir.parent_path();

        // ./Prysma --graphviz fichier.p pour activer la génération du graphe AST
        std::filesystem::create_directories(buildDir / "programme");
        if (activerGraphViz) {
            std::filesystem::create_directories(buildDir / "graphe");
        }

        std::string srcLib = (racineProjet / "src" / "Lib").string();
        std::string objLib = (buildDir / "obj" / "Lib").string();

        std::unique_ptr<RegistreFonctionGlobale> registreFonctionGlobale = std::make_unique<RegistreFonctionGlobale>();
        std::unique_ptr<RegistreFichier> registreFichiers = std::make_unique<RegistreFichier>();
        std::unique_ptr<FacadeConfigurationEnvironnement> facadeConfigurationEnvironnement = std::make_unique<FacadeConfigurationEnvironnement>(registreFonctionGlobale.get(), registreFichiers.get());
        
        OrchestrateurInclude orchestrateurInclude(registreFonctionGlobale.get(), registreFichiers.get(), mutex.get(), activerGraphViz);
        orchestrateurInclude.compilerProjet(cheminFichier);

        if (orchestrateurInclude.aDesErreurs()) {
            std::cerr << "Erreur: La compilation a échoué, arrêt du processus." << std::endl;
            return 1;
        }

        std::string nomExecutable = std::filesystem::path(cheminFichier).stem().string();

        ConstructeurSysteme::ConstructeurParams params = {
            srcLib,
            objLib,
            buildDir.string(),
            registreFichiers->obtenirTousLesFichiers(),
            nomExecutable
        };
        ConstructeurSysteme constructeur(params);
        constructeur.compilerLib();
        constructeur.lierLibExecutable();
    }
    catch (const ErreurCompilation& erreur) {
        std::cerr << nomFichier << ":" 
                  << erreur.getLigne() << ":" 
                  << erreur.getColonne() << ": Erreur: " 
                  << erreur.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}