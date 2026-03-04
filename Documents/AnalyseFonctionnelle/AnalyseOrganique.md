# Prysma: Rapport d’analyse fonctionnelle et organique

**Auteur :** Raphael Arseneault  
**Cours :** Épreuve Synthèse de programme (420-626-RK)

**Présenté à :**
- Monsieur Charles Lemaire
- Monsieur Jonathan Roy
- Monsieur Marcel Landry

**Département de techniques de l’informatique**  
*Cégep Rimouski*  
*25 mars 2026*


# Analyse Organique
## Pseudo code des algorithmes critiques 
### Algorithme de construction d’une instruction (ConstructeurArbreInstruction)
```cpp
INoeud construire(vecteur<Token>& tokens, int& index)
{
    // On récupère le parseur spécifique associé au type du token actuel dans le registre
    IParser parserAssocie = _registreInstructions.recuperer(tokens[index].type) ;
    // On délègue la tâche de parsing au parseur trouvé (ex: ParserVariable, ParserSi, etc.)
    // On passe le constructeur actuel pour permettre la récursivité
    INoeud enfant = parserAssocie.parser(tokens, index, cetteInstance) ;
    Retourner enfant ;
}
```

### Algorithme de parsing du noeud principal main
```cpp
INoeud parser(vecteur<Token>& tokens, int& index, ConstructeurArbreInstruction constructeur)
{
    NoeudMain noeudMain ;
    // On avance l'index de 2 pour sauter le mot-clé 'main' et l'accolade ouvrante '{'
    index += 2 ;
    // Tant qu'on n'est pas à la fin du fichier et qu'on ne rencontre pas l'accolade fermante '}'
    Tant que (index < tokens.taille() ET tokens[index].type != TOKEN_ACCOLADE_FERMEE)
    {
        // On demande au constructeur de bâtir l'instruction enfant à l'index actuel
        INoeud enfant = constructeur.construire(tokens, index) ;

        // On ajoute le noeud enfant généré à la liste des instructions du Main
        noeudMain.ajouterInstruction(enfant) ;
    }
    Retourner noeudMain ;
}
```

### Algorithme de construction de l'arbre d'équation récursif (ConstructeurArbreEquation)
```cpp
INoeud construire(vecteur<Token>& equation)
{
    // On nettoie l'équation en retirant les parenthèses inutiles autour de l'expression
    vecteur<Token> equationSansParentheses = _gestionnaireParenthese.enleverParenthesesEnglobantes(equation) ;
    equation = equationSansParentheses ;

    Si (equation.estVide())
    {
        Lancer Exception("Erreur: équation vide") ;
    }



    // On cherche l'opérateur avec la priorité la plus faible (racine actuelle)
    int indice = _chaineResponsabilite.trouverOperateur(equation) ;

    // Cas de base : Aucun opérateur trouvé, c'est une valeur numérique (feuille)
    Si (indice == -1)
    {
        Essayer
        {
            float valeurFloat = convertirEnFloat(equation[0].value) ;
            INoeud noeudValeur = creerNoeudValeur(valeurFloat) ;
            Retourner noeudValeur ;
        }
        Attraper (Exception e)
        {
            Lancer Exception("Erreur: impossible de convertir en nombre") ;
        }
    }



    // Cas récursif : On récupère le noeud correspondant à l'opérateur trouvé
    IExpression noeud = _registreSymbole.recupererNoeud(equation[indice].type) ;

    // On divise les tokens en deux parties : gauche et droite de l'opérateur
    vecteur<Token> gauche = extraireSousVecteur(equation, 0, indice) ;
    vecteur<Token> droite = extraireSousVecteur(equation, indice + 1, equation.fin()) ;

    // Appels récursifs pour construire les sous-arbres
    INoeud exprGauche = construire(gauche) ;
    INoeud exprDroite = construire(droite) ;

    // On lie les deux sous-arbres au noeud opérateur
    noeud.ajouterExpression(exprGauche, exprDroite) ;
    Retourner noeud ;
}
```

## Explication des entrées sortie. 
En entrée, le programme prend en charge un fichier source texte avec l'extension .prysma. Ce fichier contient les instructions du langage de haut niveau écrites par le développeur. Il est lu caractère par caractère pour être transformé en vecteur de tokens contenant les identifiants et les valeurs (string value) nécessaires à l'analyse. De plus, le compilateur accepte des arguments en ligne de commande (flags) pour configurer la compilation, comme le nom du fichier de sortie ou le niveau de verbosité du débogage.
En sortie, le compilateur génère principalement le code intermédiaire LLVM (IR) dans un fichier .ll. Ce code est la représentation bas niveau des instructions de l'arbre syntaxique. De plus, pour le débogage, le programme sort un affichage (dump) de l'AST et du code IR directement dans la console si l'argument correspondant est activé. En cas d'erreur de syntaxe, un message précis indiquant la ligne et le type d'erreur est retourné à l'utilisateur. Finalement, le backend LLVM produit le code machine exécutable optimisé pour l'architecture cible.

## Diagramme de classe 

![Diagramme de classe](../../Images/diagramme.svg) 

### Architecture Globale des Classes

C'est la représentation de l'architecture logicielle complète du compilateur Prysma. L'objectif est de structurer le compilateur autour de modules fortement découplés, respectant les principes orientés objet pour garantir la robustesse et l'évolutivité du projet.

Aspects technologiques majeurs de la hiérarchie :

* **Arbre Syntaxique Abstrait (AST)** : Défini par des interfaces souches (`INoeud`, `IInstruction`). C'est la fondation qui standardise la représentation en mémoire des variables, boucles et fonctions.
* **Patron Visiteur (Visitor)** : Résout le problème de couplage en isolant les opérations de la structure de données. L'interface `IVisiteur` permet au backend LLVM de traverser l'arbre et de générer le code intermédiaire IR sans altérer l'intégrité de l'AST.
* **Moteur d'Analyse (Parsing)** : Divisé en parseurs spécialisés et piloté par un constructeur central. Chaque instruction gère son propre cycle d'évaluation syntaxique.
* **Gestion d'État et Registres** : Utilise une approche d'isolation pour le stockage des variables, symboles et types via des registres couplés à un allocateur d'arène (Arena), ce qui permet d'optimiser les performances mémoire.

La séparation stricte des responsabilités assure la stabilité structurelle. C'est un projet évolutif, l'ajout de fonctionnalités futures s'opérera par extensions isolées.


# Patron de conception utilisée pour la conception du projet

### 1. Composite avec Interpréteur
**Description :** C'est la structure centrale de l'Arbre Syntaxique Abstrait (AST). Les interfaces souches INoeud, IExpression et IInstruction définissent un contrat unifié. Les composants primitifs (littéraux, variables) et les structures complexes (fonctions, conditions, boucles) s'emboîtent de manière hiérarchique.
**Justification/Avantages :** Résout la problématique de la représentation en mémoire de la grammaire Turing-complète. L'arbre est manipulé de manière transparente, ce qui permet d'interpréter les relations sémantiques entre les blocs avant la compilation. La construction de cet AST représente une phase de compilation essentielle pour valider la syntaxe du code et rejeter les structures invalides.

### 2. Visiteur (Visitor)
**Description :** C'est le composant qui permet l'exécution et la génération. L'interface IVisiteur est implémentée par VisiteurGeneralGenCode (pour le backend LLVM) et VisiteurGeneralGraphViz. Il traverse l'AST en appliquant une logique spécifique à chaque type de nœud, isolé des données.
**Justification/Avantages :** Sépare strictement la structure de l'AST de la logique de génération du code intermédiaire LLVM IR. Cela permet de cibler plusieurs représentations (débogage graphique, assembleur) sans altérer l'intégrité des nœuds. L'ajout de nouvelles fonctionnalités au compilateur s'opère par extensions isolées, une démonstration de fiabilité.

### 3. Chaîne de Responsabilité (Chain of Responsibility)
**Description :** C'est la solution algorithmique pour la gestion des priorités mathématiques. Les classes ChaineResponsabilite et GestionnaireOperateur agissent comme des maillons de traitement. L'équation traverse ces maillons jusqu'à l'identification et la résolution de son opérateur spécifique.
**Justification/Avantages :** Élimine la complexité des conditions imbriquées dans l'analyseur lexical et syntaxique. C'est une architecture évolutive ; nous pouvons toujours ajouter des fonctionnalités supplémentaires (comme de nouveaux opérateurs) sans réécrire la logique centrale d'évaluation.

### 4. Stratégie (Strategy)
**Description :** C'est le moteur d'évaluation dynamique du compilateur. L'interface IStrategieEquation délègue l'analyse à des stratégies concrètes (StrategieIdentifiant, StrategieAppelFonction). Le compilateur sélectionne l'algorithme d'analyse au vol selon le type de token lu.
**Justification/Avantages :** Optimise les performances du compilateur en orientant le flux d'exécution directement vers la bonne logique de construction. Résout la complexité de l'analyse syntaxique en isolant chaque comportement pour gérer les erreurs de sémantique de manière efficace.

### 5. Monteur (Builder)
**Description :** Les classes ConstructeurArbreInstruction et ConstructeurEquationFlottante orchestrent l'instanciation des nœuds de l'AST. Ils encapsulent l'API C++ de LLVM, notamment l'arène d'allocation (BumpPtrAllocator).
**Justification/Avantages :** L'apprentissage technique a démontré l'importance de maîtriser les techniques pour gérer la mémoire. Ce patron centralise la configuration des objets et prévient les fuites mémoires en s'assurant que l'AST est robuste avant de générer la résolution d'une équation.

### 6. Façade (Facade)
**Description :** La classe LlvmBackend est l'unique implémentation du patron Façade. Elle encapsule les sous-systèmes natifs de l'API C++ de LLVM (llvm::LLVMContext, llvm::Module, llvm::IRBuilder, initialisation des cibles) derrière des méthodes simplifiées et spécifiques à Prysma (creerAutoCast, declarerExterne, chargerValeur).
**Justification/Avantages :** Résout le point de blocage potentiel lié à la complexité de la génération du code par le framework LLVM. Le frontend Prysma interagit avec une interface haut niveau sans avoir à manipuler la syntaxe brute de l'assembleur LLVM IR dans chaque nœud. L'apprentissage technique de l'API LLVM est strictement isolé dans ce composant.

### 7. Objet Contexte (Context Object / Service Locator)
**Description :** La structure ContextGenCode implémente le patron Objet Contexte. Elle encapsule l'état global d'une passe de compilation et regroupe les pointeurs vers les dépendances critiques (registres d'instructions, de variables, de types, backend LLVM, et l'arène d'allocation BumpPtrAllocator).
**Justification/Avantages :** Résout le problème de la gestion d'état lors du parcours récursif de l'Arbre Syntaxique Abstrait. Au lieu de surcharger les signatures des méthodes du VisiteurGeneralGenCode avec de multiples paramètres, l'objet contexte injecte l'environnement d'exécution de manière unifiée. C'est la conception d'une architecture de compilateur robuste : elle élimine le besoin de variables globales et prépare le projet à la compilation séparée de multiples fichiers de manière thread-safe.


# Réflexion sur la complexité algorithmique (Grand O)

C'est l'analyse des algorithmes du compilateur Prysma en termes de complexité Grand O. L'objectif est de comprendre le coût réel de chaque composant du pipeline de compilation et de voir quelles améliorations de performance seraient intéressantes à intégrer dans le futur.

## 1. Analyse lexicale — Lexer::tokenizer()
**O(n)** — n = nombre de caractères du fichier source.

C'est un parcours linéaire du code source, caractère par caractère, dans une boucle while. Chaque caractère est traité une seule fois : on cherche les mots-clés dans la table motsCles (table de hachage, O(1) amorti), on classifie les opérateurs avec des switch en O(1), et on avance l'index pour les nombres et littéraux. La vérification du contexte pour les nombres négatifs passe par un std::set statique de 17 éléments, donc c'est constant en pratique. Le lexer est linéaire par rapport à la taille du fichier source.

On pourrait remplacer la logique conditionnelle par un automate fini déterministe (DFA) compilé à l'avance, ce qui éliminerait les branchements. C'est l'approche des lexers industriels comme Flex ou re2c, avec un gain estimé de 10 à 20 fois plus rapide pour la tokenisation pure.

## 2. Construction de l'arbre d'instructions — ConstructeurArbreInstruction::construire()
**O(t)** — t = nombre total de tokens.

C'est la descente récursive. La boucle principale avance l'index à chaque instruction parsée. Pour chaque token, on cherche le parseur associé dans le RegistreInstruction qui hérite de RegistreGeneric et utilise une std::map avec find() en O(log p), mais le nombre de parseurs est une constante fixe d'environ 15 types d'instructions, donc O(1) en pratique. Chaque token est consommé exactement une fois par un parseur, ce qui nous donne une complexité linéaire.

## 3. Construction de l'arbre d'équation récursif — ConstructeurArbreEquation::construire()
**O(k · e²) pire cas** — e = nombre de tokens dans l'expression, k = nombre de niveaux de priorité (13 actuellement).

C'est l'algorithme central pour les expressions mathématiques. À chaque appel récursif, la ChaineResponsabilite traverse séquentiellement les 13 maillons de priorité. Chaque maillon appelle trouverDernierAuNiveauZero() qui fait un parcours inversé du vecteur en O(e). Dans le pire cas, la chaîne parcourt les 13 niveaux avant de trouver l'opérateur de plus faible priorité, ensuite l'expression est divisée en deux et l'algorithme est appelé récursivement sur chaque moitié. La profondeur de récursion est O(e) dans le cas dégénéré (expression linéaire comme `a + b + c + d`), ce qui nous donne O(k · e²) au pire cas.

On pourrait remplacer ça par un Pratt Parser (Top-Down Operator Precedence) qui résout la priorité des opérateurs en une seule passe O(e) sans chaîne de responsabilité. C'est l'approche utilisée par V8 et Clang.

## 4. Suppression des parenthèses englobantes — ServiceParenthese::enleverParenthesesEnglobantes()
**O(e²) pire cas** — e = taille du vecteur de tokens.

La boucle while retire les parenthèses englobantes une couche à la fois. Pour chaque itération, estEnglobante() fait un parcours linéaire O(e), et les erase() sur le std::vector coûtent O(e) chacun à cause du décalage des éléments. Pour une expression comme `((((x))))`, la boucle s'exécute e/2 fois, ce qui donne du quadratique.

C'est d'ailleurs noté comme TODO dans le code source : utiliser deux index debut et fin au lieu de copier et modifier le vecteur, ce qui réduirait le coût à O(e).

## 5. Parcours en profondeur de l'AST — VisiteurBaseGenerale (Patron Visiteur)
**O(n)** — n = nombre total de nœuds dans l'AST.

C'est un parcours en profondeur classique. Chaque nœud est visité exactement une fois via accept(this). Pour NoeudInstruction on itère sur tous les enfants, pour NoeudOperation on visite gauche et droite, pour NoeudDeclarationFonction on visite les arguments puis le corps. C'est linéaire par rapport à la taille de l'arbre.

## 6. Compilation multi-fichier parallèle — OrchestrateurInclude::compilerProjet()
**O(F · C / P)** — F = nombre de fichiers, C = coût de compilation d'un fichier, P = nombre de cœurs CPU.

C'est le système de parallélisation du compilateur. La passe 1 (lexer, parseur, remplissage des registres globaux) est exécutée en parallèle avec un thread par fichier inclus. inclureFichier() utilise un std::mutex pour protéger l'ajout au vecteur de threads et la vérification des fichiers déjà traités. La passe 2 (génération LLVM) est aussi parallélisée. Le RegistreFichier utilise un std::set protégé par mutex pour vérifier les fichiers déjà compilés en O(log F).

Le problème c'est que l'approche actuelle crée un thread par fichier. Pour un gros projet avec 20 000 fichiers ça fait 20 000 threads, juste 2000 threads consomme 16 Go de RAM juste pour exister. Je pourrait utiliser un pool de threads borné au nombre de cœurs physiques avec std::thread::hardware_concurrency() et une file de tâches, c'est l'approche standard des systèmes de compilation comme Ninja et Make -j.

## 7. Lecture du fichier source — FichierLecture::entrer()
**O(n)** — n = nombre de caractères du fichier.

L'algorithme lit le fichier ligne par ligne avec std::getline() et concatène dans un std::string. Chaque document += ligne est en O(m) amorti grâce au doublement de capacité. C'est linéaire par rapport à la taille du fichier.

On pourrait utiliser mmap() pour mapper directement le fichier en mémoire virtuelle. Le lexer travaillerait directement sur le buffer mappé sans copie intermédiaire, ce qui élimine les allocations dynamiques de std::string. C'est l'approche utilisée par Clang pour le chargement des fichiers source.
