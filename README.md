# Prysma

> Ajouter vos infos supplémentaires ici à propos du choix de sujet.  
> *(Si votre projet inclut un nom de domaine, nom de style commercial, nom d'entreprise, etc.)*

## Description du projet 

> 2 paragraphes minimum

C'est un projet de développement d'un compilateur dont l'objectif sera de traduire un langage de programmation de haut niveau vers un langage intermédiaire LLVM. Le framework LLVM est largement utilisé dans l'industrie pour la compilation de code, notamment pour les langages comme le C et le C++ incluant Rust. LLVM sera la partie backend du compilateur, tandis que le frontend sera développé par moi. 

LLVM résout plusieurs problématiques liées au langage de bas niveau, comme l'assembleur. 
 - permet d'optimiser le code généré pour le rendre plus rapide et efficace en termes de mémoire.
 - Permet de cibler plusieurs architectures matérielles différentes sans avoir à réécrire le code assembleur généré pour chaque architecture. 
 - résout le problème de registres limités en utilisant une approche de registres virtuels illimités. Il gère d'ailleurs automatiquement l'allocation des registres physiques, leur stockage en mémoire RAM et les libère lorsqu'ils sont nécessaires.
 - Gère l'aspect privé de la mémoire en isolant les différentes sections, d'ailleurs on a accès à des fonctions générées par LLVM pour séparer la logique. 

En somme, le LLVM est un outil qui génère du code intermédiaire avec une syntaxe proche de l'assembleur, mais avec des fonctionnalités avancées qui facilitent la création du compilateur. 

Prysma est un langage de programmation qui inclura des fonctionnalités de base comme les variables, les fonctions, les conditions, les boucles, etc. Le compilateur devra être capable de gérer ces fonctionnalités et de générer du code LLVM correspondant. L'objectif est de créer un langage turing-complet, il y aura donc l'écriture de bibliothèques standard comme la gestion de la mémoire, arbre binaire, liste chaînée, pile, file, graphe, dictionnaire, bibliothèque de manipulation de chaîne de caractères, etc. C'est un projet évolutif, donc les fonctionnalités ne sont pas en manque, nous pouvons toujours ajouter des fonctionnalités supplémentaires au langage et au compilateur, exemple : l'orienté objet, faire un garbage collector, des lambdas, etc.


## Si possible, nommer 3 projets/produits similaires qui vous inspirent

> Qui vous inspire ou vous inspireront pour les fonctionnalités.  
> Inclure les liens vers ces projets.

| Nom du projet | Résumé | URL  |

 - Rust : langage de programmation système qui met l'accent sur la sécurité et la performance. Il utilise LLVM comme backend pour la compilation. C'est un langage très solide et a fait ses preuves dans l'industrie. D'ailleurs, le langage est à une maturation avancée, le compilateur est écrit en Rust lui-même, pour bénéficier de ses propres fonctionnalités de sécurité mémoire, c'est une démonstration de fiabilité, construire un gros projet montre sa robustesse. 
 URL du projet github : https://github.com/rust-lang/rust.git

 - Kaléidoscope : un langage de programmation éducatif, développé par l'équipe LLVM pour enseigner les concepts de base de la compilation en utilisant LLVM comme backend. C'est un bon exemple qui montre les étapes pour construire un compilateur fonctionnel. Toutes les étapes sont expliquées et documentées, ce qui facilite la compréhension. URL du tutoriel : https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/

- GCC : un compilateur open-source très populaire, il permet de compiler le C, C++, Fortran, etc. Il est largement utilisé dans l'industrie, c'est un projet mature et robuste. Intéressant de le voir pour comprendre la construction d'un compilateur évolué. Bien que les parties soient très complexes. URL du projet github : https://github.com/gcc-mirror/gcc.git


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

Cette technologie s'intègre au projet en tant que composant central, c'est le backend du compilateur. C'est la pièce qui permet la génération du code intermédiaire LLVM IR, un langage de programmation assembleur évolué pour faciliter la programmation d'un langage de programmation. Cette technologie sera utilisée pour générer le code intermédiaire pour la génération de la résolution d'une équation, ex : 20+42-432*(32-432)/3. Toute la syntaxe du langage de programmation, (if, else, for, while, <, = , == , >), utilisera cette technologie pour construire la logique qui sera compilée en assembleur par clang "NomFichier.ll" -o "sortieBinaire". 

### Indiquer les risques technologiques de votre projet

> * Où pourriez-vous bloquer?
> * Quels sont les défis techniques pour mener à terme la réalisation?

- Le débogage n'est pas seulement fait dans le langage C++ mais aussi dans le langage généré par le framework LLVM. Ce qui nous donne 2 dimensions de débogage. Si vous écrivez un algorithme dans le langage Prysma tel que une liste doublement chaînée, vous ne pouvez pas savoir si c'est l'algorithme qui n'est pas correct, si un pointeur a été mal assigné, ou si le problème vient du code C++ ou encore de la génération du code LLVM dans le fichier .ll. Ça fait beaucoup d'états de débogage où peut se trouver un problème. 
- La complexité de la génération du code par le framework LLVM peut être importante. 
- La récursivité peut être un défi, visualiser l'état d'exécution du code n'est pas aussi simple qu'avec un code non récursif. 
- La gestion de la mémoire peut être un défi, surtout dans le langage C++. Et Prysma.
- La création du garbage collector peut être un défi technique intéressant à relever. 
- La création des bibliothèques du langage Prysma.

## Illustration du projet

> Si une ou plusieurs illustrations peut nous aider à mieux comprendre le projet, les inclure ici.  
> Il est aussi possible d'inclure plus de matériel dans votre projet GitHub.



## Contenu de vos sprints

Le temps de travail pour ce projet inclut 3 sprints de 2 semaines chacun.

### Que voyez vous dans l'itération 1 du projet ?

> Listez les fonctionnalités visées à l'intérieur de 2 semaines et les technologies qui seront utilisées

### Que voyez vous dans l'itération 2 du projet ?

> Listez les fonctionnalités visées à l'intérieur de 2 semaines et les technologies qui seront utilisées

### Que voyez vous dans l'itération 3 du projet ?

> Listez les fonctionnalités visées à l'intérieur de 2 semaines et les technologies qui seront utilisées



