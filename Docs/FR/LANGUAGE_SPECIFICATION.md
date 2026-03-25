# Documentation officielle du langage Prysma 

## Introduction

Prysma est un langage système compilé via LLVM, offrant un contrôle de bas niveau avec une syntaxe moderne. Il supporte le paradigme procédural et l'orienté objet avec une gestion déterministe de la mémoire.

-----

## Syntaxe et blocs

  * **Fin d'instruction :** Chaque ligne de code doit se terminer par un point-virgule (`;`).
  * **Délimitation :** Les blocs de code (fonctions, classes, boucles) utilisent des accolades `{ }`.
  * **Identifiants :** Sensible à la casse (case-sensitive).

-----

## Types de données

| Type | Description |
| :--- | :--- |
| `int32` | Entier signé 32 bits |
| `float` | Nombre à virgule flottante 32/64 bits |
| `bool` | Valeur logique (`true`, `false`) |
| `string` | Chaîne de caractères native |
| `void` | Absence de type (retour de fonction uniquement) |
| `T[]` | Tableau de type `T` (ex: `int32[]`) |

-----

## Gestion de la mémoire

Prysma utilise une gestion **manuelle** de la mémoire sur le tas (*Heap*).

### Allocation et Libération

  * **`new` :** Alloue de la mémoire pour un objet ou un tableau.
  * **`delete` :** Libère la mémoire précédemment allouée.

<!-- end list -->

```prysma
// Allocation d'un objet
dec MaClasse instance = new MaClasse();

// Libération
delete instance;
```

-----

## Fonctions

Déclarées avec `fn` et appelées via `call`.

  * **Syntaxe :** `fn typeRetour nom(arg type nomArg)`
  * **Appel :** `call nomFonction(arguments)`

### Récursivité (Correction Factorielle)

```prysma
fn int32 factorielle(arg int32 n) {
    if (n <= 1) {
        return 1;
    }
    dec int32 suivant = n - 1;
    return n * call factorielle(suivant);
}
```

-----

## Structures de contrôle

### Conditionnelle `if...else`

```prysma
if (a > b) {
    // bloc vrai
} else {
    // bloc faux
}
```

### Boucle `while`

La condition est évaluée avant chaque itération.

```prysma
dec int32 i = 0;
while (i < 10) {
    aff i = i + 1;
}
```

-----

## Opérateurs

| Catégorie | Opérateurs |
| :--- | :--- |
| **Arithmétique** | `+`, `-`, `*`, `/`, `%` |
| **Comparaison** | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| **Logique** | `&&`, `||`, `!` |
| **Assignation** | `aff variable = valeur;` |
| **Référence** | `ref` (accès à l'adresse mémoire) |

-----

## Exemples de référence

### Création et manipulation d'un tableau dynamique

```prysma
fn void test_tableau() {
    // Déclaration d'un tableau de 5 entiers sur la pile
    dec int32[5] notes = [10, 15, 20, 12, 18];
    
    // Modification
    aff notes[0] = 20;
    
    call print(notes[0]);
}
```

### Utilisation d'une classe et polymorphisme

```prysma
fn void main() {
    // Allocation sur le tas
    dec Chien mon_chien = new Chien();
    
    call mon_chien.set_nom("Rex");
    call mon_chien.aboyer();
    
    delete mon_chien;
}
```

-----

**Note technique :** Le compilateur Prysma effectue une analyse en deux passes pour résoudre les dépendances de symboles dans les classes avant la génération du code LLVM IR. L'utilisation de `ref` est recommandée lors du passage de structures lourdes ou de chaînes de caractères aux fonctions pour éviter les copies inutiles.