# Architecture - Compilateur Prysma

## Vue d'ensemble

Prysma est un compilateur pour un langage Turing-complet générant du code intermédiaire LLVM 18. L'architecture suit une structure modulaire en trois phases : analyse lexicale (Lexer), construction de l'Arbre Syntaxique Abstrait (AST), et génération de code LLVM via le pattern Visiteur.

## Schéma général du projet

```
Code Source Prysma (.prysma)
         ↓
    [Lexer] - Tokenization (O(n))
         ↓
  Vecteur de Tokens
         ↓
 [ConstructeurArbreInstruction] - Descente Récursive (O(t))
         ↓
 [ConstructeurArbreEquation] - Récursion Mutuelle (O(k·e²))
         ↓
   Arbre Syntaxique (AST)
         ↓
  [VisiteurGeneralGenCode] - Patron Visiteur
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

2. **LecteurFichierTexte** : Extrait le code brut du fichier source et retourne une `std::string`.

3. **Lexer** : Analyse le texte et génère un vecteur de tokens (analyse lexicale).
   - Classifie les mots-clés via table de hachage `O(1) amorti`
   - Traite les opérateurs via switch en `O(1)`
   - Parcourt caractère par caractère en `O(n)` où n = nombre de caractères

4. **ConstructeurArbreInstruction** : Constructeur central piloté par descente récursive.
   - Délègue à des parseurs spécialisés via `RegistreInstruction`
   - Chaque token consommé une seule fois → `O(t)` où t = nombre de tokens

5. **ConstructeurArbreEquation** : Résout les expressions mathématiques récursivement.
   - Utilise `ChaineResponsabilite` pour identifier l'opérateur de plus faible priorité
   - Divise l'expression gauche/droite et applique la récursion
   - Complexité pire cas : `O(k·e²)` où k = niveaux de priorité (13), e = tokens expression

6. **INoeud** : Représente la logique de l'AST.
   - Interfaces souches : `INoeud`, `IInstruction`, `IExpression`
   - Construction par récursivité, chaque nœud instanciant ses enfants selon la grammaire Prysma

### Contrainte Architecturale

L'AST généré est renvoyé au Main **uniquement s'il est complet**. Cette isolation stricte applique le principe de **Fail-Fast** :
- Erreur détectée par Lexer ou ConstructeurArbreInstruction → compilation arrêtée immédiatement
- Prévient la corruption mémoire
- Garantit que LLVM reçoit un arbre syntaxique 100% valide

## Description Détaillée de l'Algorithme

### Étape 1 : Analyse Lexicale (Lexer::tokenizer)
```cpp
Lexer::tokenizer(const std::string& source) → std::vector<Token>
```
- Parcourt le fichier caractère par caractère
- Sépare mot par mot en attribuant un identifiant unique à chaque token
- Supprime les éléments superflus (espaces, caractères invalides)
- Résultat : vecteur de tokens avec identifiants et valeurs string

### Étape 2 : Construction de l'Arbre d'Instructions
```cpp
ConstructeurArbreInstruction::construire(
    std::vector<Token>& tokens, int& index
) → INoeud
```
- Parcourt le vecteur de tokens avec algorithme de descente récursive
- Pour chaque token, récupère le parseur spécifique via `RegistreInstruction`
- Délègue la parsing au parseur trouvé (ex: ParserVariable, ParserSi, ParserWhile)
- Permet la récursivité en passant le constructeur actuel au parseur
- Résultat : arbre d'instructions complet

### Étape 3 : Construction de l'Arbre d'Équations Récursif
```cpp
ConstructeurArbreEquation::construire(std::vector<Token>& equation) → INoeud
```

**Pseudo-code :**
```
fonction construire(equation : Vecteur<Token>) → INoeud
    // Nettoyer les parenthèses englobantes
    equation ← enleverParenthesesEnglobantes(equation)
    
    // Cas de base : aucun opérateur trouvé = valeur numérique
    indice ← chaineResponsabilite.trouverOperateur(equation)
    si indice == -1
        retourner creerNoeudValeur(convertirEnFloat(equation[0]))
    fin si
    
    // Cas récursif : diviser et conquérir
    operateur ← registreSymbole.recupererNoeud(equation[indice])
    gauche ← equation[0..indice-1]
    droite ← equation[indice+1..fin]
    
    // Appels récursifs
    sousArbreGauche ← construire(gauche)
    sousArbreDroite ← construire(droite)
    
    // Lier les sous-arbres
    operateur.ajouter(sousArbreGauche, sousArbreDroite)
    retourner operateur
fin fonction
```

### Étape 4 : Génération du Code LLVM via Patron Visiteur
- Utilise l'interface `IVisiteur` implémentée par `VisiteurGeneralGenCode`
- Parcourt l'AST en profondeur `O(n)` où n = nombre de nœuds
- Chaque nœud appelle `accept(this)` pour générer son IR LLVM
- Produit un fichier `.ll` (code intermédiaire LLVM)

### Étape 5 : Backend LLVM
- Optimise le code IR
- Génère l'assembleur pour l'architecture cible
- Produit l'exécutable final

## Patrons de Conception

### 1. Composite + Interpréteur (Structure AST)
**Interfaces souches :** `INoeud`, `IExpression`, `IInstruction`  
**Bénéfice :** Manipulation transparente de la grammaire Turing-complète, validation syntaxique stricte avant compilation.

### 2. Visiteur (Visitor Pattern)
**Implémentations :** `VisiteurGeneralGenCode` (LLVM IR), `VisiteurGeneralGraphViz` (débogage)  
**Bénéfice :** Sépare l'AST de la logique de génération, permet multiples représentations sans altérer les nœuds.

### 3. Chaîne de Responsabilité (Chain of Responsibility)
**Classes :** `ChaineResponsabilite`, `GestionnaireOperateur`  
**Bénéfice :** Gère automatiquement les 13 niveaux de priorité mathématique, évite les conditions imbriquées.

### 4. Stratégie (Strategy Pattern)
**Interface :** `IStrategieEquation`  
**Bénéfice :** Délègue l'analyse au vol selon le type de token (identifiant, appel fonction, etc.).

### 5. Monteur (Builder Pattern)
**Classes :** `ConstructeurArbreInstruction`, `ConstructeurArbreEquation`  
**Bénéfice :** Centralise l'instanciation des nœuds AST et l'API LLVM, prévient les fuites mémoire.

### 6. Façade (Facade Pattern)
**Classe :** `LlvmBackend`  
**Bénéfice :** Encapsule la complexité de l'API C++ LLVM derrière des méthodes haut niveau (creerAutoCast, declarerExterne, chargerValeur).

### 7. Objet Contexte (Context Object)
**Structure :** `ContextGenCode`  
**Bénéfice :** Encapsule l'état global de compilation (registres, backend, arène allocation), injecte les dépendances au visiteur sans surcharger les signatures, prépare la thread-safety multi-fichier.

## Gestion de la Mémoire

### Arena Allocator (Bump Pointer)
- Migré des `std::shared_ptr` vers LLVM `BumpPtrAllocator`
- Conversion vers pointeurs bruts (`INoeud*`)
- Avantages :
  - Mémoire contiguë → optimisation caches L1/L2
  - Fragmentation réduite
  - Risque de fuite mémoire limité
  - Performance : allocation `O(1)` amortie

### Registres Globaux
- `RegistreVariable` : stockage des déclarations de variables
- `RegistreType` : introspection des types
- `RegistreInstruction` : dispatching des parseurs
- `RegistreFichier` : gestion des inclusions (protégé par mutex)

## Optimisations Implémentées

### CMake et Clang
- Remplace les Makefile obsolètes
- Intégration de la fonction `glob` pour l'ajout automatique des chemins
- Meilleure précision des erreurs avec Clang

### Multithreading avec LLVM::ThreadPool
- Migré de `std::thread` à `llvm::ThreadPool`
- Limite les threads créés (vs création incontrôlée : 2000 threads pour 2000 fichiers)
- Séparation en deux passes :
  - **Passe 1** : Lexer, Parser, remplissage registres (parallélisé par fichier)
  - **Passe 2** : Génération LLVM (parallélisée, registres thread-safe via mutex)

### AddressSanitizer (ASAN)
- Détection immédiate des fuites mémoire et corruptions au runtime
- Drapeaux d'allocation pour trace automatique

## Sorties du Compilateur

**Fichier source :** `.prysma` (texte)  
**Fichier intermédiaire :** `.ll` (LLVM IR)  
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

2. **Construction d'équation :** Remplacer `ChaineResponsabilite` par Pratt Parser
   - Complexité améliorée : `O(k·e²)` → `O(e)`
   - Approche utilisée par V8 et Clang

3. **Suppression parenthèses :** Utiliser deux index (début/fin) au lieu de copier le vecteur
   - Complexité améliorée : `O(e²)` → `O(e)`

4. **Lecture fichier :** Utiliser `mmap()` pour mapper le fichier directement
   - Élimine les allocations dynamiques `std::string`
   - Approche utilisée par Clang

### Multithreading
- Implémenter pool de threads borné au nombre de cœurs physiques (`std::thread::hardware_concurrency()`)
- File de tâches pour dispatcher le travail
- Approche standard des systèmes comme Ninja et Make -j
