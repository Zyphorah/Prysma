# Prysma Programming Language - Development Roadmap

Ce document détaille les capacités actuelles du langage Prysma ainsi que les axes de développement futurs. Le project est structuré autour d'une architecture modulaire utilisant LLVM comme backend de compilation.

## 1. État actuel du project (Stable)

### Core Language & Syntax
* **Types Primitifs :** Support complet des entiers (`int32`), flottants (`float`) et booléens (`bool`).
* **Arithmétique :** Implémentation des opérations de base (`+`, `-`, `*`, `/`) avec gestion de la priorité des opérateurs et parenthésage.
* **Structures de Contrôle :**
    * Instructions conditionnelles `if-else` avec expressions booléennes complexes.
    * Loops `while` avec gestion de la pile via `alloca`.
* **Functions :** Support des functions récursives, passage d'arguments par valeur et gestion des types de return.
* **Gestion des Identifiants :** Déclaration de variables, assignments et résolution de portée (scoping).

### Programmation Orientée Object (POO)
* **Modèle de Classe :** Définition de structures de données et de comportements.
* **Encapsulation :** Gestion des niveaux d'accès `public`, `private` et `protected`.
* **Héritage :** Hiérarchie de classes et interfaces.
* **Polymorphisme :** Implémentation d'une **VTable** dynamique pour la résolution des méthodes virtuelles au runtime.

### Gestion de la Mémoire
* **Allocation Dynamic :** Intégration de `prysma_malloc` et `free` issus de la bibliothèque standard C.
* **Opérateurs Système :** Implémentation des mots-clés `new` et `delete` pour le contrôle manuel du tas (heap).
* **Optimisation Interne :** Utilisation d'un **Bump Pointer Allocator** (Arena) pour la gestion des nœuds de l'AST, réduisant la fragmentation et augmentant les performances de compilation.

## 2. Infrastructure & Outillage

* **Compilation Multi-threadée :** Orchestration de la compilation via `llvm::ThreadPool` pour le traitement parallèle des fichiers sources.
* **Gestion des Dépendances :** Système d'inclusion de fichiers (`#include`) pour la modularité du code.
* **Diagnostics :** Analyseur syntaxique précis rapportant les erreurs par ligne et colonne.
* **Visualisation :** Export de l'Tree Syntaxique Abstrait (AST) aux formats texte et graphique (Graphviz/DOT).
* **Écosystème IDE :** Extension VS Code pour la coloration syntaxique native du Prysma.

## 3. Roadmap de Développement (Planned Features)

### Court Terme (v1.1)
* **Standard Library (StdLib) :** Développement d'une bibliothèque de structures de données natives (listes chaînées, piles, files).
* **C Interoperability :** Système d'import automatique pour lier des bibliothèques externes C sans configuration manuelle.
* **Built-in Functions :** Intégration de functions mathématiques avancées (puissance, PGCD) au niveau du runtime.

### Moyen Terme (v1.2)
* **Gestion Mémoire Avancée :** Introduction des pointeurs pour un contrôle bas niveau granulaire.
* **Namespaces & Modules :** Organisation du code en unités de compilation isolées pour éviter les conflits de noms.
* **Generics :** Support de la programmation générique (templates) pour les functions et les classes.
* **Analyse Sémantique Étendue :** Renforcement de la vérification des types pour prévenir les erreurs de logique à la compilation.

### Long Terme (Vision)
* **Compilation JIT (Just-In-Time) :** Mode exécution immédiate pour le scripting.
* **Optimisations SSA :** Passes d'optimisation personnalisées sur l'IR LLVM.
* **Inline Assembly :** Bloc `asm` pour l'injection directe d'instructions processeur.
* **Méta-programmation :** Macros et exécution de code au moment de la compilation (Compile-time execution).