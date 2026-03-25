# Architecture - Compiler Prysma

## Vue d'ensemble

Prysma est un compiler pour un langage Turing-complet générant du code intermédiaire LLVM 18. L'architecture suit une structure modulaire en trois phases : analyse lexicale (Lexer), construction de l'Tree Syntaxique Abstrait (AST), et génération de code LLVM via le pattern Visitor.

## Schéma général du project

```
Code Source Prysma (.prysma)
         ↓
    [Lexer] - Tokenization (O(n))
         ↓
  Vecteur de Tokens
         ↓
 [BuilderTreeInstruction] - Descente Récursive (O(t))
         ↓
 [BuilderTreeEquation] - Récursion Mutuelle (O(k·e²))
         ↓
   Tree Syntaxique (AST)
         ↓
  [GeneralVisitorGenCode] - Patron Visitor
         ↓
   Code LLVM IR (.ll)
         ↓
 [LLVM Backend] - Optimisation & Compilation
         ↓
 Code Machine Exécutable
```

## Flux d'Exécution Frontend : Génération de l'AST

### Composants et Interactions

1. **Main** : Contrôleur central orchestrant le flux séquentiel sans exécuter de logique d'analyse.

2. **LecteurFileText** : Extrait le code brut du fichier source et returnne une `std::string`.

3. **Lexer** : Analyse le texte et génère un vecteur de tokens (analyse lexicale).
   - Classifie les mots-clés via table de hachage `O(1) amorti`
   - Traite les opérateurs via switch en `O(1)`
   - Parcourt caractère par caractère en `O(n)` où n = nombre de caractères

4. **BuilderTreeInstruction** : Builder central piloté par descente récursive.
   - Délègue à des parsers spécialisés via `RegistryInstruction`
   - Chaque token consommé une seule fois → `O(t)` où t = nombre de tokens

5. **BuilderTreeEquation** : Résout les expressions mathématiques récursivement.
   - Utilise `ChainOfResponsibility` pour identifier l'opérateur de plus faible priorité
   - Divise l'expression gauche/droite et applique la récursion
   - Complexité pire cas : `O(k·e²)` où k = niveaux de priorité (13), e = tokens expression

6. **INode** : Représente la logique de l'AST.
   - Interfaces souches : `INode`, `IInstruction`, `IExpression`
   - Construction par récursivité, chaque nœud instanciant ses childs selon la grammaire Prysma

### Contrainte Architecturale

L'AST généré est renvoyé au Main **uniquement s'il est complet**. Cette isolation stricte applique le principe de **Fail-Fast** :
- Erreur détectée par Lexer ou BuilderTreeInstruction → compilation arrêtée immédiatement
- Prévient la corruption mémoire
- Garantit que LLVM reçoit un tree syntaxique 100% valide

## Description Détaillée de l'Algorithme

### Étape 1 : Analyse Lexicale (Lexer::tokenizer)
```cpp
Lexer::tokenizer(const std::string& source) → std::vector<Token>
```
- Parcourt le fichier caractère par caractère
- Sépare mot par mot en attribuant un identifiant unique à chaque token
- Supprime les éléments superflus (espaces, caractères invalides)
- Résultat : vecteur de tokens avec identifiants et valeurs string

### Étape 2 : Construction de l'Tree d'Instructions
```cpp
BuilderTreeInstruction::construire(
    std::vector<Token>& tokens, int& index
) → INode
```
- Parcourt le vecteur de tokens avec algorithme de descente récursive
- Pour chaque token, récupère le parser spécifique via `RegistryInstruction`
- Délègue la parsing au parser trouvé (ex: ParserVariable, ParserSi, ParserWhile)
- Permet la récursivité en passant le builder actuel au parser
- Résultat : tree d'instructions complet

### Étape 3 : Construction de l'Tree d'Équations Récursif
```cpp
BuilderTreeEquation::construire(std::vector<Token>& equation) → INode
```

**Pseudo-code :**
```
function construire(equation : Vecteur<Token>) → INode
    // Nettoyer les parenthèses englobantes
    equation ← enleverParenthesesEnglobantes(equation)
    
    // Cas de base : aucun opérateur trouvé = valeur numérique
    indice ← chaineResponsabilite.trouverOperateur(equation)
    si indice == -1
        returnner creerNodeValeur(convertirEnFloat(equation[0]))
    fin si
    
    // Cas récursif : diviser et conquérir
    operateur ← registrySymbole.recupererNode(equation[indice])
    gauche ← equation[0..indice-1]
    droite ← equation[indice+1..fin]
    
    // Calls récursifs
    sousTreeGauche ← construire(gauche)
    sousTreeDroite ← construire(droite)
    
    // Lier les sous-trees
    operateur.ajouter(sousTreeGauche, sousTreeDroite)
    returnner operateur
fin function
```

### Étape 4 : Génération du Code LLVM via Patron Visitor
- Utilise l'interface `IVisitor` implémentée par `GeneralVisitorGenCode`
- Parcourt l'AST en depth `O(n)` où n = nombre de nœuds
- Chaque nœud callle `accept(this)` pour générer son IR LLVM
- Produit un fichier `.ll` (code intermédiaire LLVM)

### Étape 5 : Backend LLVM
- Optimise le code IR
- Génère l'assembleur pour l'architecture cible
- Produit l'exécutable final

## Patrons de Conception

### 1. Composite + Interpréteur (Structure AST)
**Interfaces souches :** `INode`, `IExpression`, `IInstruction`  
**Bénéfice :** Manipulation transparente de la grammaire Turing-complète, validation syntaxique stricte avant compilation.

### 2. Visitor (Visitor Pattern)
**Implémentations :** `GeneralVisitorGenCode` (LLVM IR), `GeneralVisitorGraphViz` (débogage)  
**Bénéfice :** Sépare l'AST de la logique de génération, permet multiples représentations sans altérer les nœuds.

### 3. Chaîne de Responsabilité (Chain of Responsibility)
**Classes :** `ChainOfResponsibility`, `ManagerOperateur`  
**Bénéfice :** Gère automatiquement les 13 niveaux de priorité mathématique, évite les conditions imbriquées.

### 4. Stratégie (Strategy Pattern)
**Interface :** `IStrategieEquation`  
**Bénéfice :** Délègue l'analyse au vol selon le type de token (identifiant, call function, etc.).

### 5. Monteur (Builder Pattern)
**Classes :** `BuilderTreeInstruction`, `BuilderTreeEquation`  
**Bénéfice :** Centralise l'instanciation des nœuds AST et l'API LLVM, prévient les fuites mémoire.

### 6. Façade (Facade Pattern)
**Classe :** `LlvmBackend`  
**Bénéfice :** Encapsule la complexité de l'API C++ LLVM derrière des méthodes haut niveau (creerAutoCast, declarerExterne, loadrValeur).

### 7. Object Context (Context Object)
**Structure :** `ContextGenCode`  
**Bénéfice :** Encapsule l'état global de compilation (registrys, backend, arène allocation), injecte les dépendances au visitor sans surloadr les signatures, prépare la thread-safety multi-fichier.

## Gestion de la Mémoire

### Arena Allocator (Bump Pointer)
- Migré des `std::shared_ptr` vers LLVM `BumpPtrAllocator`
- Conversion vers pointeurs bruts (`INode*`)
- Avantages :
  - Mémoire contiguë → optimisation caches L1/L2
  - Fragmentation réduite
  - Risque de fuite mémoire limité
  - Performance : allocation `O(1)` amortie

### Registrys Globaux
- `RegistryVariable` : stockage des déclarations de variables
- `RegistryType` : introspection des types
- `RegistryInstruction` : dispatching des parsers
- `RegistryFile` : gestion des inclusions (protégé par mutex)

## Optimisations Implémentées

### CMake et Clang
- Remplace les Makefile obsolètes
- Intégration de la fonction `glob` pour l'ajout automatique des chemins
- Meilleure précision des erreurs avec Clang

### Multithreading avec LLVM::ThreadPool
- Migré de `std::thread` à `llvm::ThreadPool`
- Limite les threads créés (vs création incontrôlée : 2000 threads pour 2000 fichiers)
- Séparation en deux passes :
  - **Passe 1** : Lexer, Parser, remplissage registrys (parallélisé par fichier)
  - **Passe 2** : Génération LLVM (parallélisée, registrys thread-safe via mutex)

### AddressSanitizer (ASAN)
- Détection immédiate des fuites mémoire et corruptions au runtime
- Drapeaux d'allocation pour trace automatique

## Outputs du Compiler

**File source :** `.prysma` (texte)  
**File intermédiaire :** `.ll` (LLVM IR)  
**Exécutable :** Sans extension (code machine)  
**Débogage optionnel :**
- Dump AST en console
- Export AST Graphviz (visualisation)
- Code IR LLVM en console

**Gestion d'erreur :**
- Messages précis avec numéro de ligne et type d'erreur
- Compilation arrêtée en cas de syntaxe invalide

## Futures Améliorations

### Optimisations Algorithmiques
1. **Lexer :** Remplacer logique conditionnelle par DFA (Deterministic Finite Automaton)
   - Gain estimé : 10-20x plus rapide que parcours linéaire

2. **Construction d'équation :** Remplacer `ChainOfResponsibility` par Pratt Parser
   - Complexité améliorée : `O(k·e²)` → `O(e)`
   - Approche utilisée par V8 et Clang

3. **Suppression parenthèses :** Utiliser deux index (début/fin) au lieu de copier le vecteur
   - Complexité améliorée : `O(e²)` → `O(e)`

4. **Reading fichier :** Utiliser `mmap()` pour mapper le fichier directement
   - Élimine les allocations dynamiques `std::string`
   - Approche utilisée par Clang

### Multithreading
- Implémenter pool de threads borné au nombre de cœurs physiques (`std::thread::hardware_concurrency()`)
- File de tâches pour dispatcher le travail
- Approche standard des systèmes comme Ninja et Make -j
