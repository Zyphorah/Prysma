https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/

Cette documentation se base sur **LLVM `IRBuilder**`, qui est la classe principale pour générer du code.

---

### I. Initialisation & Infrastructure

Ces classes ne sont pas des méthodes de génération, mais les conteneurs obligatoires.

**1. `LLVMContext**`
Le contexte global (opaque). Gère les types et les constantes uniques.

* **Syntaxe :** `llvm::LLVMContext context;`

**2. `Module**`
Le conteneur de tout votre code (fonctions, globales).

* **Syntaxe :** `new llvm::Module("nom_module", context);`

**3. `IRBuilder<>**`
Le "curseur" qui écrit les instructions.

* **Syntaxe :** `llvm::IRBuilder<> builder(context);`

---

### II. Création de Valeurs Littérales (Constantes)

Pour créer des nombres bruts (42, 3.14) dans le code.

#### 4. `llvm::ConstantInt::get`

Crée un entier constant.

* **Syntaxe :** `ConstantInt::get(Type* Ty, uint64_t V, bool IsSigned = false)`
* **Arguments :**
* `Ty` : Le type (ex: `Type::getInt32Ty(context)`).
* `V` : La valeur entière brute (ex: `42`).
* `IsSigned` : Si `true`, traite comme signé.


* **Retour :** `Value*`

#### 5. `llvm::ConstantFP::get`

Crée un flottant constant.

* **Syntaxe :** `ConstantFP::get(Type* Ty, double V)`
* **Arguments :**
* `Ty` : Type flottant (ex: `Type::getFloatTy(context)`).
* `V` : La valeur double (ex: `3.14`).


* **Retour :** `Value*`

---

### III. Système de Types

Méthodes de la classe `llvm::Type`.

#### 6. `Type::getInt32Ty(Context)`

* **Description :** Retourne le type entier 32 bits (équivalent `int`).
* **Retour :** `Type*`

#### 7. `Type::getFloatTy(Context)`

* **Description :** Retourne le type flottant 32 bits (équivalent `float`).
* **Retour :** `Type*`

#### 8. `Type::getVoidTy(Context)`

* **Description :** Retourne le type vide (pour fonctions sans retour).
* **Retour :** `Type*`

#### 9. `Type::getInt1Ty(Context)`

* **Description :** Retourne un entier 1 bit. C'est le type **booléen** obligatoire pour toutes les conditions (`if`, `while`).
* **Retour :** `Type*`

---

### IV. Déclaration de Fonctions

#### 10. `FunctionType::get`

Définit la signature d'une fonction (Prototype).

* **Syntaxe :** `FunctionType::get(Type* Result, ArrayRef<Type*> Params, bool isVarArg)`
* **Arguments :**
* `Result` : Type de retour.
* `Params` : Vecteur des types des arguments.
* `isVarArg` : `false` (sauf si fonction variadique comme `printf`).


* **Retour :** `FunctionType*`

#### 11. `Function::Create`

Crée la fonction attachée au module.

* **Syntaxe :** `Function::Create(FunctionType* Ty, LinkageTypes Linkage, const Twine &N, Module *M)`
* **Arguments :**
* `Ty` : Le type créé ci-dessus.
* `Linkage` : `Function::ExternalLinkage` (visible partout).
* `N` : Nom de la fonction (string).
* `M` : Le module parent.


* **Retour :** `Function*`

#### 12. `BasicBlock::Create`

Crée un bloc de code (label). Une fonction doit en avoir au moins un ("entry").

* **Syntaxe :** `BasicBlock::Create(Context, const Twine &Name, Function *Parent)`
* **Arguments :**
* `Context` : Le contexte global.
* `Name` : Nom du bloc (ex: "entry", "if.then").
* `Parent` : La fonction à laquelle ce bloc appartient.


* **Retour :** `BasicBlock*`

#### 13. `builder.SetInsertPoint`

Déplace le curseur d'écriture à la fin d'un bloc.

* **Syntaxe :** `builder.SetInsertPoint(BasicBlock *TheBB)`
* **Usage :** Absolument nécessaire avant de commencer à écrire des instructions dans un nouveau bloc.

---

### V. Gestion de la Mémoire (Variables Mutables)

Utilisation de l'IRBuilder.

#### 14. `CreateAlloca`

Alloue de la mémoire sur la pile (Stack). Déclaration de variable locale.

* **Syntaxe :** `CreateAlloca(Type *Ty, Value *ArraySize = nullptr, const Twine &Name = "")`
* **Arguments :**
* `Ty` : Le type de la variable à stocker (ex: `Int32`).
* `ArraySize` : `nullptr` (sauf pour tableaux).
* `Name` : Nom pour le débogage.


* **Retour :** `AllocaInst*` (qui est un pointeur vers le type Ty).

#### 15. `CreateStore`

Écrit une valeur dans une adresse mémoire (Assignation).

* **Syntaxe :** `CreateStore(Value *Val, Value *Ptr)`
* **Arguments :**
* `Val` : La valeur à écrire (ex: le résultat de `1+1`).
* `Ptr` : L'adresse mémoire (résultat du `CreateAlloca`).


* **Retour :** `StoreInst*`

#### 16. `CreateLoad`

Lit une valeur depuis une adresse mémoire.

* **Syntaxe :** `CreateLoad(Type *Ty, Value *Ptr, const Twine &Name = "")`
* **Arguments :**
* `Ty` : Le type de donnée attendu (OBLIGATOIRE depuis LLVM 14+).
* `Ptr` : L'adresse mémoire source.
* `Name` : Nom de la variable temporaire créée.


* **Retour :** `LoadInst*` (La valeur brute).

---

### VI. Arithmétique Entière

#### 17. `CreateAdd` (Addition)

* **Syntaxe :** `CreateAdd(Value *LHS, Value *RHS, const Twine &Name = "")`
* **Arguments :** `LHS` (Gauche), `RHS` (Droite). Doivent être de type Entier.
* **Retour :** `Value*`

#### 18. `CreateSub` (Soustraction)

* Identique à `CreateAdd`.

#### 19. `CreateMul` (Multiplication)

* Identique à `CreateAdd`.

#### 20. `CreateSDiv` (Division Signée)

* Division pour entiers signés (int standard).
* **Note :** Utilisez `CreateUDiv` pour les non-signés (unsigned).

---

### VII. Arithmétique Flottante (Float)

**Attention :** Ne mélangez pas Int et Float.

#### 21. `CreateFAdd` (Addition Float)

* **Syntaxe :** `CreateFAdd(Value *LHS, Value *RHS, ...)`
* Attention au 'F' préfixe.

#### 22. `CreateFSub` (Soustraction Float)

* Identique à `CreateFAdd`.

#### 23. `CreateFMul` (Multiplication Float)

* Identique à `CreateFAdd`.

#### 24. `CreateFDiv` (Division Float)

* Identique à `CreateFAdd`.

---

### VIII. Comparaisons (Pour les `if`)

Retourne toujours un `Int1` (Booléen).

#### 25. `CreateICmpEQ` (Entier ==)

* **Syntaxe :** `CreateICmpEQ(Value *LHS, Value *RHS)`
* Teste l'égalité entière.

#### 26. `CreateICmpNE` (Entier !=)

* Teste l'inégalité entière.

#### 27. `CreateICmpSLT` (Entier <)

* Signed Less Than. (Utiliser `ULT` pour Unsigned).

#### 28. `CreateICmpSGT` (Entier >)

* Signed Greater Than.

#### 29. `CreateFCmpOEQ` (Float ==)

* **Ordered Equal**. "Ordered" signifie que la comparaison échoue si l'un des nombres est NaN.

#### 30. `CreateFCmpOLT` (Float <)

* Ordered Less Than.

---

### IX. Contrôle de Flux (Sauts)

#### 31. `CreateBr` (Saut Inconditionnel)

* **Syntaxe :** `CreateBr(BasicBlock *Dest)`
* **Usage :** Pour aller à la fin d'un `if` ou boucler dans un `while`.

#### 32. `CreateCondBr` (Saut Conditionnel)

* **Syntaxe :** `CreateCondBr(Value *Cond, BasicBlock *True, BasicBlock *False)`
* **Arguments :**
* `Cond` : Doit être un `Int1` (résultat d'un `ICmp` ou `FCmp`).
* `True` : Bloc où aller si Vrai.
* `False` : Bloc où aller si Faux.



#### 33. `CreateRet` (Retour Valeur)

* **Syntaxe :** `CreateRet(Value *V)`
* Termine la fonction et retourne `V`.

#### 34. `CreateRetVoid` (Retour Vide)

* **Syntaxe :** `CreateRetVoid()`
* Pour les fonctions `void`.

---

### X. Conversions (Casting)

LLVM est strictement typé. Impossible d'additionner un Int et un Float sans cast explicite.

#### 35. `CreateSIToFP` (Int -> Float)

* **Syntaxe :** `CreateSIToFP(Value *Val, Type *DestTy)`
* Convertit un entier Signé (`int`) en point Flottant.

#### 36. `CreateFPToSI` (Float -> Int)

* **Syntaxe :** `CreateFPToSI(Value *Val, Type *DestTy)`
* Convertit un Float en entier Signé (troncature).

#### 37. `CreateBitCast` (Réinterprétation)

* Change le type sans changer les bits (dangereux, usage avancé).

---

### XI. Outils de Validation

#### 38. `llvm::verifyFunction`

Vérifie que la fonction générée est valide (pas de sauts morts, pas de types incohérents).

* **Syntaxe :** `verifyFunction(*function, &llvm::errs())`
* **Retour :** `bool` (true si erreur).

#### 39. `module->print`

Affiche le code IR généré (ASM) dans la console.

* **Syntaxe :** `module->print(llvm::errs(), nullptr)`

#### 40. `Function::arg_begin` / `arg_end`

Itérateurs pour récupérer les arguments passés à la fonction lors de la génération de son corps.

* Utilisé pour lier les paramètres de fonction (`int a`) à des variables locales (`Alloca`).