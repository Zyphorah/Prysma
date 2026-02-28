#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Registre/RegistreFichier.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <filesystem>
#include <cstdlib>
#include <memory>
#include <mutex>

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

// Explication du fonctionnnement pour la nouvelle refactorisation des registres pour supporter le multi thread: 
// Je vais faire une première passe qui va construire des registres globaux pour les fonctions, et les variables
// Ce registre contiendra une initialisation nullptr pour les objets de llvm llmv::Value llvm::allocainstance. 
// Ensuite je vais faire une classe locale pour l'initialisation des objets nullptr en mémoire dans le registre. 
// Cette façon de procéder est utilisée pour éviter les conflit entre les threads de mon programme. 

// Niveau de la génération du code locale je vais pouvoir faire du multi thread pour générer le code en parallèle pour chaque fichier. Chaque fichier 
// sera traité avec leur propre thread pour la génération du code et au final tout les fichier généré seront rassemblé pour faire un lien final. 

// Les variables globales devront être initialisé dans le visiteur 2 non avec un nullptr car elle sont global dans une fonction précise donc pas de problème au niveau 
// des autres thread. 

// Le premier visiteur est seulement un éclaireur pour remplir les registres de fonction et de variable il remplit les registres avec des valeur    
// nullptr pour les fonction et les variables


// Le premier visiteur c'est le visiteur de remplissage de registre, VisiteurRemplissageRegistre.cpp

// Ne pas oblier les contexte objet llvm sont strictement privé et ne peuvent pas être partagé entre eux

// Faire un système d'héritage pour les visiteur, une classe de base qui contient toute les définition de visiteur 
// ça va éviter d'avoir une dupplication de code, ou une définition de contrat pour les visiteur que je veux seulement faire une action précise, 
// exemple remplir un registre de fonction, ou remplir un registre de variable, tout les registres peuvent être rempli en avance puis ensuite 
// utiliser une technique de multi threade pour faire la génération de code en parallèle, le but est d'avoir un système découplé pour ne pas 
// avoir de problème de race condition. 

// Bon actuellement j'ai un problème au niveau des threads si j'ai 20 000 fichier je vais avoir 20 000 thread ce qui 
// Est un volume de thread énorme abusé, juste 2000 thread consomme 16 go de ram juste pour exister 
// C'est un problème pour les gros projet car cela fera crash l'ordinateur, je vais devoir dans le future 
// faire un système de pool de thread pour limiter le nombre de thread au nombre de coeur de la machine. 
// Dans le context de mon ordinateur actuelle par exemple j'ai 32 coeurs ce qui serait optimal 32 thread pour compiler en 
// parallèle les fichiers source. 
// Une solution pourrait être d'avoir un pool de thread et envoyer une stack de fichier à compiler pour les threads 
// Ce qui évite de faire planter le pc pour la compilation d'un gros projet avec beaucoup de fichier source. 
// Une solution à implémenter dans le future 


#include <llvm/Support/TargetSelect.h>


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
                  << erreur.ligne << ":" 
                  << erreur.colonne << ": Erreur: " 
                  << erreur.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}