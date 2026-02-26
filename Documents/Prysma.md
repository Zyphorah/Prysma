# Documentation officielle du langage de programmation Prysma

## Table des matières
1. [Introduction](#introduction)
2. [Syntaxe générale](#syntaxe-générale)
3. [Types de données](#types-de-données)
4. [Déclaration de variables](#déclaration-de-variables)
5. [Fonctions](#fonctions)
6. [Structures de contrôle](#structures-de-contrôle)
7. [Commentaires](#commentaires)
8. [Opérateurs](#opérateurs)
9. [Exemples pratiques](#exemples-pratiques)

---

## Introduction

Prysma est un langage de programmation compilé dont la syntaxe s'inspire des langages comme C++, Java, Rust au niveau de la syntaxe et des concepts. Il a été conçu avec une architecture lexicale robuste pour faciliter le traitement et la compilation du code vers LLVM.

**Caractéristiques principales:**
- Typage statique fort avec types primitifs et tableaux
- Syntaxe claire et lisible
- Support des fonctions avec paramètres nommés et références
- Mot-clé `fn` pour la déclaration des fonctions
- Support des scopes et de l'organisation modulaire
- Compilation vers LLVM pour portabilité maximale

---

## Syntaxe générale

La syntaxe de Prysma suit les conventions des langages modernes:
- Les instructions se terminent par un point-virgule (`;`)
- Les blocs de code sont délimités par des accolades (`{}`)
- La casse est sensible (case-sensitive)
- Les espaces et indentations améliorent la lisibilité
---

## Types de données

Prysma supporte les types de données suivants:

| Type | Description | Exemple |
|------|-------------|---------|
| `int32` | Entier signé 32 bits | `10`, `-5`, `0` |
| `int64` | Entier signé 64 bits | `42`, `-100` |
| `float` | Nombre à virgule flottante | `20.5`, `-3.14` |
| `string[]` | Tableau de caractères (chaîne) | `"Bonjour"`, `"Prysma"` |
| `bool` | Booléen | `true`, `false` (représentés par 1 ou 0) |
| `void` | Aucun retour | Utilisé pour les fonctions |
| `[]` | Tableaux | `int32[]`, `float[]`, `bool[]` |

---

## Déclaration de variables

Les variables en Prysma sont déclarées avec le mot-clé `dec`, suivi du type, du nom et optionnellement d'une valeur initiale. Pour modifier une variable existante, utilisez le mot-clé `aff`.

### Syntaxe:
```prysma
dec type nomVariable = valeur;
aff nomVariable = nouvelleValeur;
```

### Exemples:
```prysma
dec int32 entier = 10;
dec float flottant = 20.5;
dec string[] texte = "Bonjour, Prysma!";
dec bool booleen = true;
aff entier = 42;
```

### Références:
Pour utiliser une référence vers une variable, utilisez le mot-clé `ref`:
```prysma
dec int32 a = 10;
dec int32 b = ref a;  // b référence a
```

---

## Fonctions

Les fonctions en Prysma sont définies avec le mot-clé `fn`. Pour appeler une fonction, utilisez le mot-clé `call`.

### Syntaxe:
```prysma
fn typeRetour nomFonction(arg type param1, arg type param2, ...) 
{
    // Corps de la fonction
    return valeur;
}

// Appel de fonction
call nomFonction(param1, param2);
```

### Exemples:

#### Fonction avec paramètres et retour:
```prysma
fn int32 addition(arg int32 a, arg int32 b) 
{
    dec int32 resultat = a + b;
    return resultat;
}

// Utilisation
dec int32 x = call addition(5, 3);
```

#### Fonction sans paramètres:
```prysma
fn int32 obtenir_nombre() 
{
    return 42;
}
```

#### Fonction avec plusieurs paramètres:
```prysma
fn float calculer_moyenne(arg float note1, arg float note2, arg float note3) 
{
    dec float somme = note1 + note2 + note3;
    return somme / 3.0;
}
```

#### Fonction sans retour:
```prysma
fn void afficher_message(arg string[] message) 
{
    call print(ref message);
}
```

#### Fonction récursive (Fibonacci):
```prysma
fn int32 fib(arg int32 n) 
{
    if (n <= 1) 
    {
        return n;
    }
    else
    {
        dec int32 un = n - 1;
        dec int32 deux = n - 2;
        dec int32 fib_un = call fib(un);
        dec int32 fib_deux = call fib(deux);
        dec int32 res = fib_un + fib_deux;
        return res;
    }
}
```

---

## Structures de contrôle

### Conditionnelle `if...else`

Exécute du code selon une condition booléenne.

```prysma
if (condition) 
{
    // Code exécuté si la condition est vraie
} 
else 
{
    // Code exécuté si la condition est fausse
}
```

**Exemple:**
```prysma
fn bool testIntSup() 
{
    dec int32 a = 5;
    if (a > 3) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}
```

### Boucle `while`

Répète du code tant qu'une condition est vraie.

```prysma
while (condition) 
{
    // Code à exécuter tant que la condition est vraie
}
```

**Exemple:**
```prysma
fn bool testBoucleSimple() 
{
    dec int32 i = 0;
    while (i < 5) 
    {
        aff i = i + 1;
    }
    if (i == 5) 
    {
        return true;
    }
    return false;
}
```

---

## Commentaires

### Commentaires sur une seule ligne
```prysma
// Ceci est un commentaire sur une seule ligne
dec int32 x = 5;  // Ce commentaire explique la ligne
```

### Notes sur les commentaires
- Les commentaires commencent par `//`
- Utiles pour documenter le code et expliquer la logique complexe
- Ignores par le compilateur
## Opérateurs

### Opérateurs arithmétiques

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `+` | Addition | `a + b` |
| `-` | Soustraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `%` | Modulo | `a % b` |

### Opérateurs de comparaison

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `==` | Égal à | `a == b` |
| `!=` | Différent de | `a != b` |
| `<` | Inférieur à | `a < b` |
| `>` | Supérieur à | `a > b` |
| `<=` | Inférieur ou égal à | `a <= b` |
| `>=` | Supérieur ou égal à | `a >= b` |

### Opérateurs logiques

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `&&` | ET logique | `a && b` |
| `\|\|` | OU logique | `a \|\| b` |

### Opérateur d'assignation

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `=` | Assignation | `aff a = 5` |

### Ordre de priorité des opérateurs

1. `*`, `/`, `%` (Multiplication, Division, Modulo)
2. `+`, `-` (Addition, Soustraction)
3. `<`, `>`, `<=`, `>=` (Comparaisons)
4. `==`, `!=` (Égalité)
5. `&&` (ET logique)
6. `\|\|` (OU logique)

---

## Exemples pratiques

### Exemple 1: Programme simple avec affichage

```prysma
include "test.p";

fn int64 main() 
{
    dec int64 a = 42;
    call print(a);
    return 0;
}
```

### Exemple 2: Fonction avec paramètres

```prysma
fn void teste(arg int32 a, arg int32 b)
{
    call printInt(a);
    call printInt(b);
}

fn int32 main()
{
    dec int32 e = 14;
    dec int32 f = 15;
    call teste(e, f);
    return 0;
}
```

### Exemple 3: Boucles et conditionnelles

```prysma
fn bool testBoucleConditionExtern()
{
    dec int32 i = 0;
    dec bool condition = i < 5;

    while(condition)
    {
        aff i = i + 1;
        aff condition = i < 5;
    }
    
    if (i == 5)
    {
        return true;
    }
    return false;
}
```

### Exemple 4: Récursivité - Factorielle

```prysma
fn int32 fact_accum(arg int32 produit, arg int32 n)
{
    aff produit = produit * n;
    call printInt(produit);
    dec int32 suivant = n + 1;
    call fact_accum(produit, n);
    return produit;
}

fn int32 main()
{
    dec int32 resultat = 1;
    dec int32 nombre = 2;
    call fact_accum(resultat, nombre);
    return resultat;
}
```

### Exemple 5: Tableaux

```prysma
fn bool tableauTestLecture_int()
{
    dec string[] fonctionnalite = "Lecture tableau : ";
    call print(ref fonctionnalite);

    dec int32[4] tableau = [0, 1, 2, 3];

    if (tableau[0] != 0) {
        return false;
    }
    if (tableau[1] != 1) {
        return false;
    }
    return true;
}
```

### Exemple 6: Tableau avec affectation

```prysma
fn bool tableauTestAffectation_int()
{
    dec string[] fonctionnalite = "Affectation tableau : ";
    call print(ref fonctionnalite);

    dec int32[4] tableau = [0, 1, 2, 3];

    aff tableau[0] = 10;
    aff tableau[1] = 11;

    if (tableau[0] != 10) {
        return false;
    }
    return true;
}
```

---

## Notes supplémentaires

### Convention de nommage recommandée

- **Variables et fonctions:** `snake_case`
  - Exemples: `nombre_entier`, `nom_utilisateur`, `calculer_moyenne`
- **Constantes:** `UPPER_SNAKE_CASE`
  - Exemples: `MAX_ITERATIONS`, `DEFAULT_VALUE`

