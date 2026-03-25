# Documentation officielle du langage Prysma 

## Introduction

Prysma est un langage système compilé via LLVM, offrant un contrôle de bas niveau avec une syntaxe moderne. Il supporte le paradigme procédural et l'orienté object avec une gestion déterministe de la mémoire.

-----

## Syntaxe et blocs

  * **Fin d'instruction :** Chaque ligne de code doit se terminer par un point-virgule (`;`).
  * **Délimitation :** Les blocs de code (functions, classes, boucles) utilisent des accolades `{ }`.
  * **Identifiants :** Sensible à la casse (case-sensitive).

-----

## Types de données

| Type | Description |
| :--- | :--- |
| `int32` | Integer signé 32 bits |
| `float` | Nombre à virgule flottante 32/64 bits |
| `bool` | Valeur logique (`true`, `false`) |
| `string` | Chaîne de caractères native |
| `void` | Absence de type (return de fonction uniquement) |
| `T[]` | Array de type `T` (ex: `int32[]`) |

-----

## Gestion de la mémoire

Prysma utilise une gestion **manuelle** de la mémoire sur le tas (*Heap*).

### Allocation et Libération

  * **`new` :** Alloue de la mémoire pour un object ou un array.
  * **`delete` :** Libère la mémoire précédemment allouée.

<!-- end list -->

```rust
// Allocation d'un object
dec MaClasse instance = new MaClasse();

// Libération
delete instance;
```

-----

## Functions

Déclarées avec `fn` et callées via `call`.

  * **Syntaxe :** `fn typeReturn nom(arg type nomArg)`
  * **Call :** `call nomFunction(arguments)`

### Récursivité (Correction Factorielle)

```rust
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

```rust
if (a > b) {
    // bloc vrai
} else {
    // bloc faux
}
```

### Loop `while`

La condition est évaluée avant chaque itération.

```rust
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
| **Comparison** | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| **Logique** | `&&`, `||`, `!` |
| **Assignation** | `aff variable = valeur;` |
| **Référence** | `ref` (accès à l'adresse mémoire) |

-----

## Exemples de référence

### Création et manipulation d'un array dynamique

```rust
fn void test_array() {
    // Déclaration d'un array de 5 entiers sur la pile
    dec int32[5] notes = [10, 15, 20, 12, 18];
    
    // Modification
    aff notes[0] = 20;
    
    call print(notes[0]);
}
```

### Utilisation d'une classe et polymorphisme

```rust
fn void main() {
    // Allocation sur le tas
    dec Chien mon_chien = new Chien();
    
    call mon_chien.set_nom("Rex");
    call mon_chien.aboyer();
    
    delete mon_chien;
}
```

-----

**Note technique :** Le compiler Prysma effectue une analyse en deux passes pour résoudre les dépendances de symboles dans les classes avant la génération du code LLVM IR. L'utilisation de `ref` est recommandée lors du passage de structures lourdes ou de chaînes de caractères aux functions pour éviter les copies inutiles.