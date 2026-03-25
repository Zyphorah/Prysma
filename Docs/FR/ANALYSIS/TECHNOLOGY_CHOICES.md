# Analyse Technique : Project Prysma

Ce document fournit une analyse détaillée du développement du langage Prysma, couvrant les aspects théoriques, les besoins functionnels, les défis de bas niveau et l'architecture du compiler.

## 1. Objectifs du project

L'objectif principal est de développer un langage nommé "Prysma" répliquant les functionnalités modernes. Le compiler est écrit en C++. Pour la génération de code, trois approches ont été analysées :

- **LLVM IR (Retenu) :** Représentation intermédiaire en SSA (Single Static Assignment). Facilite les optimisations et la génération machine multi-cibles.
- **Backend x86-64 Manuel :** Offre un contrôle total mais impose une gestion complexe des registrys et de l'ABI.
- **Transpilation vers C :** Simple à implémenter mais limite le contrôle fin sur les optimisations spécifiques.

Remarque : LLVM IR est une représentation intermédiaire (IR) en SSA, indépendante de la cible, facilitant les optimisations et la génération de code pour différentes architectures. Utiliser LLVM IR permet de ne pas gérer soi-même toutes les complexités de la génération machine.

Exemple de LLVM IR cible :

```llvm
%1 = load i32, i32* %ptr_x        ; load x depuis %ptr_x
%resultat = add i32 %1, 5         ; additionne 5 à la valeur chargée
```

## 2. Défis de la Génération de Code Machine

### Problématiques du Backend personnalisé (x86-64 Linux)

1. **Gestion des registrys :** L'architecture x86-64 offre 16 registrys généraux. Il faut gérer efficacement l'utilisation et l'allocation pour éviter les conflits et les sauvegardes inutiles.
2. **Allocation de registrys :** Nécessite des algorithmes sophistiqués (coloriage de graphe, Linear Scan) ou une gestion du débordement sur la pile (spilling).
3. **Table des symboles et Adressage :** Implique la gestion manuelle des adresses et des offsets de cadre (stack frame).
4. **Conventions d'call (ABI) :** Implémentation manuelle du passage des arguments, de l'alignement de la pile et du return de valeur (System V AMD64).
5. **Encapsulation et visibilité :** Nécessite des conventions (préfixes, sections) pour contrôler la visibilité des symboles à la liaison.
6. **Complexité :** Écrire et maintenir un backend assembleur est plus long et sujet aux erreurs qu'utiliser un backend établi.

### Avantages de l'infrastructure LLVM

- **Abstraction et Portabilité :** Le compiler peut cibler plusieurs architectures sans modifications majeures.
- **Optimisations :** LLVM intègre de nombreuses optimisations avancées appliquées automatiquement au code intermédiaire.
- **Écosystème :** Riche en outils (analyse statique, génération de symboles de débogage DWARF).
- **Gestion des registrys :** LLVM utilise des registrys virtuels en forme SSA, simplifiant l'allocation.

## 3. Projects similaires inspirants

 - Rust : langage de programmation système qui met l'accent sur la sécurité et la performance. Il utilise LLVM comme backend pour la compilation. C'est un langage très solide et a fait ses preuves dans l'industrie. D'ailleurs, le langage est à une maturation avancée, le compiler est écrit en Rust lui-même, pour bénéficier de ses propres functionnalités de sécurité mémoire, c'est une démonstration de fiabilité, construire un gros project montre sa robustesse. 
 URL du project GitHub : https://github.com/rust-lang/rust.git

 - Kaléidoscope : un langage de programmation éducatif, développé par l'équipe LLVM pour enseigner les concepts de base de la compilation en utilisant LLVM comme backend. C'est un bon exemple qui montre les étapes pour construire un compiler functionnel. Toutes les étapes sont expliquées et documentées, ce qui facilite la compréhension. URL du tutoriel : https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/

- GCC : un compiler open-source très populaire, il permet de compiler le C, C++, Fortran, etc. Il est largement utilisé dans l'industrie, c'est un project mature et robuste. Intéressant de le voir pour comprendre la construction d'un compiler évolué. Bien que les parties soient très complexes. URL du project GitHub : https://github.com/gcc-mirror/gcc.git
