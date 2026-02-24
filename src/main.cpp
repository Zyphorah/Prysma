#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/GestionnaireErreur.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <filesystem>
#include <cstdlib>
#include <memory>

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

int main(int argc, char* argv[])
{

   
    if (argc < 2) {
        std::cerr << "Erreur: Aucun fichier spécifié" << std::endl;
        return 1;
    }
    
  
    std::string cheminFichier = argv[1];
    std::string nomFichier = std::filesystem::path(cheminFichier).string(); 
    
    try {
        
        // Le seule registre qui sera utilisée globalement par tout le monde
        std::unique_ptr<RegistreFonction> registreFonctionGlobale = std::make_unique<RegistreFonction>();
        std::unique_ptr<FacadeConfigurationEnvironnement> facadeConfigurationEnvironnement = std::make_unique<FacadeConfigurationEnvironnement>(registreFonctionGlobale.get());
        
        OrchestrateurInclude orchestrateurInclude(facadeConfigurationEnvironnement.get(), registreFonctionGlobale.get());
        orchestrateurInclude.nouvelleInstance(cheminFichier);

        // Link les fichiers ensemble 
        ConstructeurSysteme constructeur("../src/Lib", "Lib", "output.ll", "programme");
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