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
// llvm::ThreadPool

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

// Actuellement j'ai un problème au niveau des threads si j'ai 20 000 fichier je vais avoir 20 000 thread ce qui 
// Est un volume de thread énorme abusé, juste 2000 thread consomme 16 go de ram juste pour exister 
// C'est un problème pour les gros projet car cela fera crash l'ordinateur, je vais devoir dans le future 
// faire un système de pool de thread pour limiter le nombre de thread au nombre de coeur de la machine. 
// Dans le context de mon ordinateur actuelle par exemple j'ai 32 coeurs ce qui serait optimal 32 thread pour compiler en 
// parallèle les fichiers source. 
// Une solution pourrait être d'avoir un pool de thread et envoyer une stack de fichier à compiler pour les threads 
// Ce qui évite de faire planter le pc pour la compilation d'un gros projet avec beaucoup de fichier source. 
// Une solution à implémenter dans le future 

// Jinja2 pour la génération du code source 

// FUTAMURA projection pour faire un compilateur qui se compile lui même


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Erreur: Aucun fichier spécifié" << std::endl;
        std::cerr << "Usage: Prysma <fichier.p> [--graphviz]" << std::endl;
        return 1;
    }
    
    std::string cheminFichier;
    bool activerGraphViz = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
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

        ConstructeurSysteme constructeur(srcLib, objLib, buildDir.string(), registreFichiers->obtenirTousLesFichiers(), nomExecutable);
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