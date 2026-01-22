# Prysma

> Ajouter vos infos supplémentaires ici à propos du choix de sujet.  
> *(Si votre projet inclus un nom de domaine, nom de style commercial, nom d'entreprise, etc.  )*

## Description du projet 

> 2 paragraphes minimum

C'est un projet de développement d'un compilateur dont l'objectif sera de traduire un langage de programmation de haut niveau vers un langage intermédiaire LLVM. Le framework LLVM est largement utilisé dans l'industrie pour la compilation de code, notamment pour les langages comme le C et le C++ en incluent le rust. LLVM sera la partie backend du compilateur, tandis que le frontend sera développé par moi. 

LLVM résous plusieurs problématiques liées au langage de bas niveau comme l'assembleur. 
 - permet d'optimiser le code générer pour le rendre plus rapide et efficace en terme de mémoire.
 - Permet de cibler plusieurs architectures matérielles différentes sans avoir à réécrire le code assembleur générer pour chaque architecture. 
 - résous le problème de registre limité en utilisant une approche de registre virtuel ilimité. Il gère d'ailleurs automatiquement l'allocation des registres physiques en mémoire ram et les sort lorsqu'il sont en besoin.
 - Gère l'aspect privé de la mémoire en isolant les différentes sections, d'ailleurs on à accès à des fonctions généré par le LLVM pour séparer la logique. 

En somme, le LLVM est un outil qui génère du code intermédiaire avec un syntaxe proche de l'assembleur, mais avec des fonctionnalités avancées qui facilitent la création du compilateur. 

Prysma est un langage de programmation qui inclura des fonctionnalités de base comme les variables, les fonctions, les conditions, les boucles, etc. Le compilateur devra être capable de gérer ces fonctionnalités et de générer du code LLVM correspondant. L'objectif est de créer un langage turing-complet, il y aura donc l'écriture de bibliothèques standard comme la gestion de la mémoire, arbre binaire, liste chaînée, pile, file, graphe, dictionnaire, bibliothèque de manipulation de chaîne de caractères, etc. C'est un projet évolutif, donc les fonctionnalités ne sont pas en manque, nous pouvons toujours ajouter des fonctionnalités supplémentaires au langage et au compilateur, exemple l'orienté objet, faire un garbage collector, des lamdas, etc.


## Si possible, nommer 3 projets/produits similaires qui vous inspirent

> Qui vous inspire ou vous inspireront pour les fonctionnalités.  
> Inclure les liens vers ces projets.

| Nom du projet | Résumé | URL  |

 - Rust : langage de programmation système qui met l'accent sur la sécurité et la performance. Il utilise LLVM comme backend pour la compilation. C'est un langage très solide et à fait ses preuves dans l'industrie. D'ailleurs, le langage est à une maturation avancée, le compilateur est écrit en Rust lui-même, pour 
 bénéficier de ses propres fonctionnalités de sécurité mémoire, c'est une demonstration de fiabilité, construire un gros projet montre sa robustesse. 
 URL du projet github : https://github.com/rust-lang/rust.git

 - Kaléidoscope : un langage de programmation éducatif, développé par l'équipe LLVM pour enseigner les concepts de base de la compilation en utilisant LLVM comme backend. C'est un bon exemple qui montre les étapes pour construire un compilateur fonctionnel. Toute les étapes sont expliquées et documentées ce qui facilite la compréhension. URL du tutoriel : https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/

- GCC : un compilateur open-source très populaire, il permet de compiler le C, C++, Fortran, etc. Il est largement utilisée dans l'industrie, c'est un projet mature et robuste. Interessant de le voir pour comprendre la construction d'un compilateur évoluer. Bien que les partie sois très complexe. URL du projet github : https://github.com/gcc-mirror/gcc.git


## Liste du matériel nécessaire pour ce projet

> Indiquer le matériel dont vous avez besoin.  
>
> * Est-ce que tous les éléments matériels nécessaires au projet sont en votre possession ?  Sinon, que vous manque-t-il ?
> * Aimeriez-vous que le département vous prête un composant, une pièce ? Si oui laquelle ?

| Matériel / Pièce | URL |
| -------------------- | --------------------------- |
|                      |                             |

## Contrainte d'acceptation

### Identifier les deux aspects technologiques 

> Une liste d'aspects technologiques vous est fournie dans le plan de cours.

| Aspect technologique | Explication (si nécessaire) |
| -------------------- | --------------------------- |
| Algorithmique        |                             |
|                      |                             |

### Préciser l'apprentissage technique qui se fera pendant le projet

> Soyez aussi précis que possible (nom de la technologie, de la librairie, url, etc.)

| Technologie | Résumé | URL  |
| ----------- | ------ | ---- |

LLVM : 
    Le projet utilisera le framework LLVM pour la génération de code intermédiaire. L'apprentissage portera sur l'utilisation de l'API C++ de LLVM pour créer des modules, des fonctions, des blocs de base et des instructions. Il sera également nécessaire de comprendre comment manipuler les types de données, les constantes et les variables dans LLVM. De plus, l'optimisation du code généré sera un aspect important à maîtriser pour améliorer les performances du compilateur.
URL : https://llvm.org/

#### Indiquer comment cette technologie s'intègre à votre projet

> Au moins 1 paragraphe

Cette technologie s'intègre au projet en tant que composant central, c'est le backend du compilateur. 

### Indiquer les risques technologiques de votre projet

> * Où pourriez-vous bloquer?
> * Quels sont les défis techniques pour mener à terme la réalisation?



## Illustration du projet

> Si une ou plusieurs illustrations peut nous aider à mieux comprendre le projet, les inclure ici.  
> Il est aussi possible d'inclure plus de matériel dans votre projet GitHub.



## Contenu de vos sprints

Le temps de travail pour ce projet inclus 3 sprints de 2 semaines chacun.

### Que voyez vous dans l'itération 1 du projet ?

> Listez les fonctionnalités visées à l'intérieur de 2 semaines et les technologies qui seront utilisées

### Que voyez vous dans l'itération 2 du projet ?

> Listez les fonctionnalités visées à l'intérieur de 2 semaines et les technologies qui seront utilisées

### Que voyez vous dans l'itération 3 du projet ?

> Listez les fonctionnalités visées à l'intérieur de 2 semaines et les technologies qui seront utilisées



