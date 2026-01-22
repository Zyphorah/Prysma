# Analyse Technique : Projet Prysma

Ce document fournit une analyse détaillée du développement du langage Prysma, couvrant les aspects théoriques, les besoins fonctionnels, les défis de bas niveau et l'architecture du compilateur.

## 1. Objectifs du projet

L'objectif principal est de développer un langage nommé "Prysma" répliquant les fonctionnalités modernes. Le compilateur est écrit en C++. Pour la génération de code, trois approches ont été analysées :

- **LLVM IR (Retenu) :** Représentation intermédiaire en SSA (Single Static Assignment). Facilite les optimisations et la génération machine multi-cibles.
- **Backend x86-64 Manuel :** Offre un contrôle total mais impose une gestion complexe des registres et de l'ABI.
- **Transpilation vers C :** Simple à implémenter mais limite le contrôle fin sur les optimisations spécifiques.

Remarque : LLVM IR est une représentation intermédiaire (IR) en SSA, indépendante de la cible, facilitant les optimisations et la génération de code pour différentes architectures. Utiliser LLVM IR permet de ne pas gérer soi-même toutes les complexités de la génération machine.

Exemple de LLVM IR cible :

```llvm
%1 = load i32, i32* %ptr_x        ; charge x depuis %ptr_x
%resultat = add i32 %1, 5         ; additionne 5 à la valeur chargée
```

## 2. Défis de la Génération de Code Machine

### Problématiques du Backend personnalisé (x86-64 Linux)

1. **Gestion des registres :** L'architecture x86-64 offre 16 registres généraux (RAX, RBX, RCX, RDX, RBP, RSP, RSI, RDI, R8–R15). Certains ont des conventions d'usage spécifiques. Il faut gérer efficacement l'utilisation et l'allocation pour éviter les conflits et les sauvegardes inutiles.

2. **Allocation de registres :** On peut utiliser des algorithmes sophistiqués comme le coloriage de graphe (graph coloring) ou "Linear Scan" pour l'allocation. Une approche naïve consiste à déborder sur la pile (spilling) et à utiliser la pile pour stocker les temporaires quand il n'y a pas assez de registres.

3. **Table des symboles et Adressage :** En assembleur, on manipule des adresses et des offsets (par rapport à RBP/RSP). Il faut gérer les accès mémoire via des tables de symboles et des offsets de cadre (stack frame) pour éviter les conflits de nommage.

4. **Conventions d'appel (ABI) :** Il faut implémenter manuellement la convention d'appel (passage des arguments dans les registres/pile, sauvegarde des registres callee-saved, alignment de la pile, retour de la valeur). Il est essentiel de respecter l'ABI cible (System V AMD64 sur Linux) pour garantir l'interopérabilité.

5. **Encapsulation et visibilité :** Les notions de "public" et "private" n'existent pas directement en assembleur. Il faut mettre en place des conventions (préfixes, sections, symboles locaux/globaux) pour contrôler la visibilité des symboles à la liaison.

6. **Complexité et productivité :** Écrire et maintenir un backend assembleur est plus long et sujet aux erreurs qu'utiliser un backend établi (comme LLVM). Les compilateurs modernes en C/C++ intègrent des optimisations avancées difficiles à reproduire rapidement.

### Avantages de l'infrastructure LLVM

- **Abstraction et Portabilité :** Indépendance vis-à-vis de l'architecture cible. Le compilateur peut cibler plusieurs architectures (x86-64, ARM, etc.) sans modifications majeures.
- **Optimisations :** LLVM intègre de nombreuses optimisations avancées appliquées automatiquement au code intermédiaire, améliorant les performances.
- **Écosystème :** Riche en outils et bibliothèques facilitant le développement (analyse statique, instrumentation, génération de symboles de débogage DWARF).
- **Gestion des registres et symboles :** LLVM utilise des registres virtuels et gère le nommage des variables, éliminant les conflits et simplifiant l'allocation.

En résumé : utiliser LLVM IR réduit énormément la charge d'implémentation et les risques, tandis qu'un backend assembleur personnalisé offre un contrôle total mais impose un effort de développement bien plus important.

## 3. Conception du Langage Prysma

### Syntaxe et Sémantique

Prysma utilise une syntaxe claire basée sur l'indentation pour définir les blocs (Off-side rule).

```rust
// Variables et Types
int x = 5 
string message = "Bonjour, Prysma!"
char caractere = 'A'
bool estVrai = true

// Contrôle de flux (Indentation définit le bloc)
if x = 0 
if x < 0
if x != 0
if x > 0
    print("x est positif")
else
    print("x est nul ou négatif")

// Boucle For avec syntaxe d'itérateur
for i in 0..10
    print(i)

// Boucle While
while x < 100
    x = x * 2

// Fonctions (mot-clé 'def' pour faciliter le parsing)
def int add_function(int a, int b)
    int result = a + b
    return result

// Appel de fonction
int somme = add_function(10, 20)
```

### Conception Orientée Objet (optionnel)

- **Classes et objets :** Définition de la syntaxe et de la sémantique pour la création de classes et l'héritage.
- **Méthodes et attributs :** Implémentation de la déclaration et l'appel de méthodes.
- **Encapsulation :** Gestion de la visibilité des membres (public, private, protected).
- **Polymorphisme :** Support de la redéfinition de méthodes dans les classes dérivées.

### Analyse Sémantique

- **Table des Symboles :** Utilisation d'une pile de tables de hachage pour gérer les portées imbriquées (Scopes). Chaque bloc indenté pousse une nouvelle portée.
- **Vérification de Type :** Validation statique des opérations binaires et des signatures de fonctions.

## 4. Gestion de la Mémoire (Garbage Collector)

L'implémentation d'un ramasse-miettes propriétaire est prévue pour automatiser la gestion du tas (Heap).

### Technique du Shadow Stack

LLVM peut cacher des pointeurs dans des registres optimisés, les rendant invisibles pour un scanneur de pile classique. Prysma utilise une **Shadow Stack** :

- **Principe :** Une pile logicielle gérée manuellement où chaque pointeur vers un objet vivant est enregistré.
- **Raison :** Garantir que le GC identifie toujours les racines (Roots) malgré les abstractions de LLVM sur la RAM et les registres.

### Algorithme Mark-and-Sweep

1. **Phase de Marquage (Mark) :** Parcours récursif depuis la Shadow Stack. Marquage d'un bit dans le *header* de l'objet (8 octets de métadonnées).
2. **Phase de Balayage (Sweep) :** Parcours linéaire du tas et libération des objets non marqués.

## 5. Implémentation du Compilateur

### Architecture générale

- **Frontend :** Lexer (gestion de l'indentation via une pile d'espaces) et Parser.
- **AST (Abstract Syntax Tree) :** Utilisation du design pattern **Composite** pour représenter les nœuds. Chaque nœud stocke sa position (ligne/colonne) pour les diagnostics d'erreurs.
- **Analyse Sémantique :** Parcours de l'AST pour la vérification de types et la construction de la table des symboles.
- **Génération de code :** Parcours de l'AST et appel à l'API `llvm::IRBuilder` pour produire l'IR.
- **Runtime :** Le binaire final est lié à une bibliothèque d'exécution écrite en C++ gérant le Shadow Stack et l'allocation initiale.

### Lexer et Gestion de l'Indentation

Le lexer gère l'indentation via une pile d'espaces, générant des tokens spéciaux (INDENT, DEDENT) pour délimiter les blocs.

### Parser et Construction de l'AST

Le parser utilise une approche récursive descendante (Recursive Descent) pour construire l'arbre syntaxique. Les nœuds AST implémentent le pattern Composite.

### Traversée de l'AST pour Génération LLVM IR

Chaque nœud AST possède une méthode `codegen()` qui :
- Pour une variable : génère une charge (`load`) ou une allocation (`alloca`).
- Pour une fonction : crée une fonction LLVM et génère le corps.
- Pour une boucle : génère les blocs de contrôle (entrée, corps, sortie).

---

**Note :** La structure complète du compilateur permet de gérer efficacement les aspects de bas niveau via LLVM, réduisant la complexité tout en maintenant un contrôle fin sur la sémantique du langage.

