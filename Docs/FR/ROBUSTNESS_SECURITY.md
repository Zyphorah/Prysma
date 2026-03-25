# Robustesse et Sécurité - Compiler Prysma

## Vue d'ensemble

Ce document détaille les mesures de sécurité et de robustesse implémentées dans le compiler Prysma pour anticiper et gérer les cas limites, les erreurs de mémoire, et les conditions de concurrence.

## 1. Construction de l'Tree d'Équation - Gestion des Cas Invalides

**Composant :** `BuilderTreeEquation::construire()`  
**Criticité :** Haute (composant le plus vulnérable aux entrées syntaxiques invalides)

### Cas Limite 1 : Parenthèses Vides ou Asymétriques

**Scenario :** 
```rust
dec int x = ();
dec int y = (5 + );
dec int z = ((5 + 3);
```

**Protection Implémentée :**
- `ServiceParenthese::enleverParenthesesEnglobantes()` valide d'abord l'intégrité des parenthèses
- Si l'expression résultante est vide après suppression → **exception lancée immédiatement**
- Prévient les accès pointeurs invalides et l'indéfinition des variables
- L'AST n'est pas construit, la compilation s'arrête au stade frontend

**Code :**
```cpp
if (equation.empty()) {
    throw std::runtime_error(
        "Error: Empty equation after parenthesis removal at line " + 
        std::to_string(token.line)
    );
}
```

### Cas Limite 2 : Opérateurs Consécutifs sans Opérande

**Scenario :**
```rust
dec int result = 5 + * 3;
dec int bad = 10 / / 2;
```

**Protection Implémentée :**
- `ChainOfResponsibility` divise l'expression à chaque opérateur trouvé
- Isole un opérateur orphelin (ex: `*` sans gauche/droite)
- Call à `convertirEnFloat()` échoue sur le token invalide
- **Exception sémantique lancée** plutôt que génération d'IR LLVM corrompue
- Erreur signalée à l'utilisateur avec ligne et contexte

**Exemple d'erreur :**
```
Syntax Erreur at line 5: Invalid operand after operator '+' 
Expected expression, got operator '*'
```

### Cas Limite 3 : Débordement de Pile (Stack Overflow) sur Expressions Complexs

**Scenario :**
```rust
// Expression très profondément imbriquée
dec int x = ((((((((((a + b) + c) + d) + e) + f) + g) + h) + i) + j)));
```

**Protection Implémentée :**
- `BuilderTreeEquation` utilise la récursion mutuelle contrôlée
- Chaque call divise l'expression en deux parties (gauche/droite)
- Depth de récursion ≤ nombre d'opérateurs = O(e) borné
- Pas de limite artificielle, mais la structure de l'équation force une division équilibrée
- Débordement mémoire peu probable en pratique pour code lisible

## 2. Compilation Multi-File Parallèle - Gestion de la Concurrence

**Composant :** `OrchestratorInclude::compilerProject()`  
**Criticité :** Critique (synchronisation multi-thread, intégrité mémoire)

### Cas Limite 1 : Dépendances Circulaires

**Scenario :**
```
file_a.prysma includes file_b.prysma
file_b.prysma includes file_c.prysma
file_c.prysma includes file_a.prysma  ← boucle infinie sans protection
```

**Protection Implémentée :**
- `RegistryFile` maintient un `std::set<std::string>` des fichiers compilés
- Protégé par `std::mutex` pour la thread-safety
- À chaque call `inclureFile()` :
  1. Vérifie si le fichier est déjà présent dans le set
  2. Si présent → ignore l'inclusion (prévient la boucle infinie)
  3. Si absent → ajoute à la liste et déclenche la compilation
- Chaque vérification est en `O(log F)` où F = nombre de fichiers

**Code Simplifié :**
```cpp
bool RegistryFile::estDejaCompile(const std::string& nomFile) {
    std::lock_guard<std::mutex> lock(mutexFiles);
    if (fichiers.find(nomFile) != fichiers.end()) {
        return true;  // Déjà compilé, ignore
    }
    fichiers.insert(nomFile);
    return false;  // Nouveau, doit être compilé
}
```

**Résultat :**
- Prévient création infinie de threads
- Évite saturation RAM (2000 threads = ~16 GB)
- Évite débordement de pile (Stack Overflow)
- Dépendances circulaires gérées silencieusement

### Cas Limite 2 : Exception Levée dans un Thread Parallèle

**Scenario :**
```
Passe 1 (Analyse) : 5 fichiers en parallèle
File 1 : ✓ Succès
File 2 : ✗ Erreur de syntaxe à la ligne 42
File 3 : ✓ Succès
File 4 : En cours...
File 5 : En cours...
```

**Protection Implémentée :**
- Le thread fautif capture l'exception et signale l'erreur au contexte global
- Flag d'erreur stocké dans `ContextGenCode`
- Après la première passe, vérification du flag d'erreur
- Si erreur détectée → passe 2 (génération LLVM) verrouillée et annulée
- Call `join()` sur tous les threads pour préserver l'intégrité des arènes allocation

**Code Simplifié :**
```cpp
void OrchestratorInclude::compilerPasse1() {
    for (auto& thread : threads) {
        try {
            // Compilation parallèle
            thread = std::thread([this]() { 
                compilerfichier(); 
            });
        } catch (const std::exception& e) {
            contextGenCode.setError(true);
            contextGenCode.setMessageError(e.what());
        }
    }
    
    // Attendre tous les threads
    for (auto& thread : threads) {
        thread.join();  // Sûr : préserve arènes
    }
    
    if (contextGenCode.hasError()) {
        return;  // Passe 2 annulée
    }
}
```

**Résultat :**
- Pas de comportement indéfini
- Tous les threads se terminent proprement
- Ressources libérées correctement
- Message d'erreur précis à l'utilisateur

### Cas Limite 3 : Race Condition sur Registrys Partagés

**Scenario :**
```
Thread 1 : Écrit dans RegistryVariable
Thread 2 : Lit depuis RegistryVariable
→ Reading d'état incohérent
```

**Protection Implémentée :**
- Registrys protégés par `std::mutex` pour accès exclusif
- Polymorphisme statique pour application conditionnelle des locks
- Passe 1 (analyse) : accès concurrent protégé
- Passe 2 (génération LLVM) : peut être rendue single-threaded si nécessaire

**Résultat :**
- Pas de race conditions
- État cohérent garanti
- Petite latence de synchronisation acceptable

## 3. Analyse Lexicale - Sécurité de Reading Mémoire

**Composant :** `Lexer::tokenizer()`  
**Criticité :** Haute (porte d'entrée des données non validées)

### Cas Limite 1 : File Source Vide ou Espaces Seuls

**Scenario :**
```
file.prysma → 0 octets (fichier vide)
ou
file.prysma → "     \n\n    " (espaces et returns à la ligne)
```

**Protection Implémentée :**
- Loop de tokenisation atteint la fin du fichier sans générer de symboles
- Vecteur de tokens returnné est vide
- `BuilderTreeInstruction` reçoit vecteur vide, ignore la boucle de construction du Main
- Compiler génère **exécutable valide et minimal** qui returnne `0`
- Pas de segfault, pas de comportement indéfini

**Code :**
```cpp
std::vector<Token> Lexer::tokenizer(const std::string& source) {
    std::vector<Token> tokens;
    size_t i = 0;
    
    while (i < source.length()) {
        if (std::isspace(source[i])) {
            i++;
            continue;  // Ignore espaces
        }
        // ... tokenisation normale
    }
    
    return tokens;  // Peut être vide, c'est valide
}
```

### Cas Limite 2 : Chaîne de Caractères Non Fermée en Fin de File

**Scenario :**
```rust
main {
    dec string message = "Hello World
}  ← Fin de fichier sans guillemet fermant
```

**Protection Implémentée :**
- Loop d'extraction de chaîne valide condition `i < source.length()` à chaque itération
- Fin prématurée du fichier force l'interruption avant guillemet fermant
- **Erreur lexicale signalée** avec numéro de ligne
- Prévient :
  - Buffer Overread (reading hors mémoire)
  - Loop infinie
  - Token corrompu

**Code :**
```cpp
if (source[i] == '"') {
    std::string value;
    i++;  // Skip ouverture
    
    while (i < source.length() && source[i] != '"') {
        value += source[i];
        i++;
    }
    
    if (i >= source.length()) {
        // ← Chaîne non fermée détectée AVANT fin du code
        throw LexerException(
            "Unterminated string starting at line " + 
            std::to_string(currentLine)
        );
    }
    
    i++;  // Skip fermeture
    tokens.push_back(Token(TOKEN_STRING, value, currentLine));
}
```

### Cas Limite 3 : Caractères Non-ASCII ou Encodage Invalide

**Scenario :**
```
file.prysma → "Déclarer int x = 5;"  (accents non attendus)
ou
file.prysma → données binaires corrompues
```

**Protection Implémentée :**
- Lexer parcourt caractère par caractère
- Classification via `std::isspace()`, `std::isdigit()`, comparaisons directes
- Caractères non reconnus ignorés silencieusement ou classés comme identifiants
- Pas d'accès hors limites car boucle valide toujours `i < source.length()`
- Comportement dégradé acceptable : caractères invalides supprimés

## 4. Conversion de Types - Sécurité des Alignements Mémoire

**Composant :** `BuilderTreeEquation`, génération LLVM  
**Criticité :** Critique (errors invisibles : segmentation fault sans trace)

### Cas Limite : Float 64-bit assigné à Float 32-bit

**Scenario :**
```
float64 (8 octets, align 8) → float32 (4 octets, align 4)
Alignement mémoire incompatible : segmentation fault silencieux
```

**Bug Historique Documenté :**
Tentative d'assigner une valeur 64-bit dans un registry 32-bit causait :
- Segmentation fault sans message d'erreur
- Erreur invisible : code machine généré était syntaxiquement valide
- Symptôme : le programme crash aléatoirement

**Protection Implémentée :**
- Système de **casting dynamique explicite** avant assignation
- Vérification des alignements en LLVM IR (attributs `align`)
- AddressSanitizer (ASAN) avec flags `-fsanitize=address`
- Détection immédiate au runtime :
  ```
  =================================================================
  ==12345==ERROR: AddressSanitizer: attempting double-free on 0x...
  ```

**Code :**
```cpp
llvm::Value* ContextGenCode::creerAutoCast(
    llvm::Value* valeur,
    llvm::Type* typeDestination
) {
    llvm::Type* typeSource = valeur->getType();
    
    if (typeSource == typeDestination) {
        return valeur;  // Pas de cast nécessaire
    }
    
    // Cast explicite selon les types
    if (typeSource->isFloatTy() && typeDestination->isDoubleTy()) {
        return builder->CreateFPExt(valeur, typeDestination);
    } else if (typeSource->isDoubleTy() && typeDestination->isFloatTy()) {
        return builder->CreateFPTrunc(valeur, typeDestination);
    }
    // ... autres cas
    
    throw std::runtime_error("Invalid cast requested");
}
```

## 5. Passage d'Arguments - Gestion de Portée

**Composant :** `RegistryArgument`, calls de fonction  
**Criticité :** Haute (corruption mémoire en contextes imbriqués)

### Cas Limite : Calls Imbriqués avec Registry Partagé

**Scenario :**
```rust
dec int testArgDanIf(int param) {
    if (param > 0) {
        return call testArgDanIf(param - 1);
    }
    return 0;
}
```

**Bug Historique :**
- Function interne vidait le `RegistryArgument` partagé
- Valeur de return attendue par fonction parente était corrompue
- Résultat : nombre aléatoire au lieu de la valeur calculée

**Protection Implémentée :**
- **Pile de contexte** (`std::stack<ContextLocal>`)
- Chaque call de fonction crée un contexte local isolé
- Variables privées par fonction garanties
- Vidange stricte des registrys après chaque call
- Portée garantie par scope RAII

**Code Simplifié :**
```cpp
class RegistryArgument {
private:
    std::stack<std::vector<llvm::Value*>> pile_contexte;
    
public:
    void inputrFunction() {
        pile_contexte.push({});  // Nouveau contexte vide
    }
    
    void ajouterArgument(llvm::Value* val) {
        pile_contexte.top().push_back(val);
    }
    
    void sortirFunction() {
        pile_contexte.pop();  // Context détruit proprement
    }
};
```

**Résultat :**
- Pas de corruption mémoire
- Récursion correctement gérée
- Valeurs de return intactes

## 6. Identifiants GraphViz - Prévention d'Écrasement d'ID

**Composant :** `GeneralVisitorGraphViz`  
**Criticité :** Moyenne (affecte le débogage, pas l'exécution)

### Cas Limite : Tree Profond avec ID Statiques

**Scenario :**
```
Tree très profond (100+ niveaux)
Identifiants statiques réutilisés → collision
Représentation visuelle faussée : nœuds fusionnés à tort
```

**Bug Historique :**
Ancien système d'ID statiques écrasait les IDs dans les trees profonds, faussant la représentation Graphviz.

**Protection Implémentée :**
- Remplacement par `std::stack<int>` de compteurs d'ID
- Chaque nœud visité incrémente le compteur du contexte
- ID unique garantie : stack de depth = depth du nœud
- Pas de collision même pour trees très profonds

**Code :**
```cpp
class GeneralVisitorGraphViz {
private:
    std::stack<int> pile_id;
    int compteur_global = 0;
    
public:
    void visit(INode* node) {
        int id_courant = compteur_global++;
        pile_id.push(id_courant);
        
        // ... traitement du nœud
        // ID unique : (depth, compteur_global)
        
        pile_id.pop();
    }
};
```

**Résultat :**
- Pas de collision d'ID
- Représentation Graphviz correcte
- Débogage visuel fiable

## Résumé des Mesures de Sécurité

| Composant | Cas Limite | Mécanisme de Protection | Résultat |
|-----------|-----------|------------------------|----------|
| BuilderTreeEquation | Parenthèses vides | Exception immédiate | AST non corrompu |
| BuilderTreeEquation | Opérateurs orphelins | Validation sémantique | Erreur précise à l'utilisateur |
| OrchestratorInclude | Dépendances circulaires | Set + mutex | Pas de boucle infinie |
| OrchestratorInclude | Exception dans thread | Flag d'erreur global | Passe 2 annulée |
| Lexer | File vide | Gestion vecteur vide | Exécutable valide |
| Lexer | Chaîne non fermée | Vérification limite | Buffer overread évité |
| Conversion Types | Alignement mémoire | Auto-cast + ASAN | Segfault détecté |
| RegistryArgument | Calls imbriqués | Pile de contexte | Pas de corruption |
| VisitorGraphViz | ID statiques | Stack d'ID unique | Pas de collision |

## Futures

1. **Ajout de tests de fuzzing** : Générer aléatoirement des entrées invalides pour découvrir edge cases
2. **Couverture mémoire complète** : Utiliser ValGrind pour détecter les fuites résiduelles
3. **Timeout de compilation** : Prévenir les boucles infinies de compilation
4. **Validation de schéma** : Vérifier la conformité des fichiers `.prysma` avant parsing
