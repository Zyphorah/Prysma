# Journal de Développement - Architecture et Décisions Techniques (Prysma)

## 1. Refonte de l'Arbre Syntaxique Abstrait (AST) et des Visiteurs
* **Injection de dépendances (Visiteur Général) :** Le passage du contexte par constructeur exigeait des modifications sur plus de 30 emplacements distincts pour une seule fonctionnalité, générant des bogues liés à des objets manquants ou non initialisés. L'injection directe du `ContextGenCode` dans le visiteur général centralise la boîte à outils. Cette architecture élimine la transmission fastidieuse par les parseurs et réduit le temps de débogage.
* **Récursion Mutuelle :** L'ancien algorithme d'extraction d'équations, basé sur le balayage manuel des virgules et des parenthèses, était inefficace et provoquait des plantages (ex. : intégration d'un appel de fonction dans une addition). Son remplacement par un système de récursion mutuelle entre `ConstructeurArbreEquation` et `ConstructeurArbreInstruction` délègue naturellement la logique d'arrêt et la résolution des expressions imbriquées.
* **Typage et Abstraction :** L'utilisation de LLVM et de la réflexion pour deviner les types a été supprimée. Le système de registre encapsule désormais `llvm::Value*` avec une abstraction `IType*`. Le compilateur connaît exactement le type et l'adresse mémoire, éliminant la rigidité du typage.
* **Génération dynamique des nœuds :** Le remplacement des nœuds manuels par une usine automatique utilisant un fichier YAML abstrait accélère l'ajout de nœuds. La déduction des labels par types a été automatisée dans le script Python, nettoyant le `ast.yaml`.
* **Remplacement des Dynamic Casts :** Les `dynamic_cast` ont été retirés au profit d'une génération dynamique de code AST via des enums générés automatiquement, augmentant la vitesse d'identification des nœuds.

## 2. Gestion de la Mémoire et LLVM
* **Arena Allocator (Bump Pointer) :** La gestion de la mémoire a migré des `std::shared_ptr` vers un système d'allocation par aréna fourni par LLVM. La conversion vers des pointeurs bruts (`INoeud*`) rend la mémoire contiguë. Les performances des caches L1/L2 sont optimisées, la fragmentation est réduite, et le risque de fuite de mémoire est limité. 
* **Sauts mémoire redondants :** La correction d'une erreur d'architecture sur le chargement des pointeurs a résolu des erreurs de segmentation. Lors du passage de valeurs en arguments de fonction, le système copie désormais uniquement le pointeur (8 octets) plutôt que la structure complète.
* **Tableaux par composition :** L'architecture des tableaux a été modifiée pour utiliser la composition au lieu de l'héritage. L'utilisation de tailles dynamiques plutôt que de valeurs codées en dur (ex. : `constValue`) offre une flexibilité totale pour le stockage de variables.
* **Décompilation LLVM :** L'analyse du code machine LLVM intermédiaire via décompilation a servi de base documentaire principale pour l'implémentation de structures complexes (ex. : logique de branchement `if/else`, alignement des octets, comportement des tableaux en mémoire).

## 3. Optimisations et Flux de travail
* **CMake et Compilateur Clang :** L'abandon des fichiers `Makefile` obsolètes (qui nécessitaient deux passes `cmake` puis `make` causant des conflits) et la configuration exclusive sous Clang ont amélioré la précision des erreurs. L'intégration de la fonction `glob` dans le `CMakeLists.txt` a supprimé l'ajout manuel des chemins d'inclusion, permettant un gain de temps direct.
* **Multithreading et LLVM::ThreadPool :** L'utilisation initiale de `std::thread` provoquait la création incontrôlée de threads (ex. : 2000 threads pour 2000 fichiers). La migration vers `llvm::ThreadPool` délègue la gestion de l'instanciation au pool, stabilisant la compilation de masse. La séparation de l'orchestrateur en deux passes prévient les conflits d'accès.
* **Thread Safety des Registres :** L'extraction des objets LLVM (`llvm::Value`, `llvm::Function`, `llvm::AllocaInst`) de la construction des registres a réglé les problèmes de *thread safety*. Un polymorphisme statique a été implémenté pour l'application conditionnelle de `std::mutex`.
* **Sanitizer (ASAN) :** L'implémentation d'AddressSanitizer ajoute des drapeaux d'allocation permettant l'affichage immédiat des fuites de mémoire au *runtime*. 
* **VTable et Classes :** Le constructeur de *vtable* a été déplacé à la passe 2 dans `constructeurEnvironnementFonction.cpp`. Lors de la première passe, les pointeurs de fonction n'existent pas encore en mémoire, ce qui entraînait le remplissage d'un registre vide.

## 4. Corrections de Bogues Majeurs Documentés
* **Bogue de l'assignation Float :** L'erreur de segmentation lors de l'assignation d'un float 64 bits à un float 32 bits a été identifiée grâce aux attributs d'alignement de LLVM IR. Un système de *casting* dynamique a été implémenté.
* **Bogue de portée d'argument (If) :** L'appel imbriqué `call testArgDanIf(param)` provoquait une corruption de la mémoire. La fonction interne vidait le `RegistreArgument` partagé et corrompait la valeur de retour attendue par la fonction parente. Corrigé par la gestion stricte de la vidange des registres.
* **Bogue de synchronisation des variables :** Les lectures ont été descendues au niveau du nœud plutôt que dans le parseur pour régler les désynchronisations lors des recherches dans le `RegistreVariable`.
* **Bogue des identifiants (GraphViz) :** Le remplacement des identifiants (ID) de nœuds statiques par une pile (`std::stack`) dans le visiteur GraphViz a corrigé un bogue où l'ancien système écrasait les ID dans les arbres profonds, faussant la représentation visuelle.

## 5. Algorithmique et Conception 
* **Mots-clés et Constexpr :** La vérification des types dans le parseur et la définition des mots-clés du lexer ont migré d'un `std::unordered_set` vers des tableaux statiques `constexpr`, optimisant le temps de compilation.
* **Templates Matcher :** Le système de *template matcher* variadique a été initié pour automatiser la génération du code répétitif.
* **Dette Technique :** Des décisions pragmatiques ont été prises, telles que le report de l'implémentation des listes chaînées. Les algorithmes réalistes ont été priorisés face à l'instabilité de la gestion des pointeurs du compilateur en cours de développement. Les valeurs codées en dur (ex. : types de retour) ont été progressivement éliminées grâce au `RegistreType` et à l'introspection.

## 6. Stratégie de débogage avec LLVM

* **Decompilation LLVM IR pour la documentation :** J'ai adopté une technique de compilation du code C++ pour en extraire le code LLVM généré par la version 18 exactement. Cela me permet de mieux comprendre les subtilités de LLVM et d'éviter les hallucinations des IA qui généralisent souvent avec des exemples provenant de versions antérieures (LLVM 14 par exemple) qui ne fonctionnent pas avec ma version actuelle.

* **Sanitizers pour le débogage invisible :** Les erreurs de segmentation provoquées par des alignements mémoire incorrect (float 64-bit vs 32-bit) étaient invisibles. L'implémentation d'AddressSanitizer (--sanitize de clang) a permis d'identifier immédiatement les fuites et corruptions mémoire au runtime.

## 7. Prochaines priorités pour les itérations suivantes

* **Permetter le stockage des retours de fonction :** Je dois implémenter la capacité à stocker le résultat d'un appel de fonction dans une variable. C'est un manque majeur pour la prochaine itération.

* **Passage de nombres bruts en arguments :** Le bug sur les appels `call(1,1)` reste à corriger. Actuellement, seules les variables passent correctement ; les constantes numériques bloquent.

* **Programmation Orientée Objet :** Le système de classes avec polymorphisme via vtable et la bibliothèque standard (listes chaînées, arbres binaires, piles, files) sont les grands défis des itérations futures.