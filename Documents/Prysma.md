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

Prysma est un langage de programmation moderne dont la syntaxe s'inspire des langages comme C++, Java et Python. Il a été conçu avec une architecture lexicale robuste pour faciliter le traitement et la compilation du code.

**Caractéristiques principales:**
- Typage statique fort
- Syntaxe claire et lisible
- Support des fonctions et structures de contrôle classiques
- Mot-clé `def` pour la déclaration des fonctions (choix architectural)

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
| `int` | Entier signé | `10`, `-5`, `0` |
| `float` | Nombre à virgule flottante | `20.5`, `-3.14` |
| `string` | Chaîne de caractères | `"Bonjour"`, `"Prysma"` |
| `bool` | Booléen | `true`, `false` |
| `void` | Aucun retour | Utilisé pour les fonctions |

---

## Déclaration de variables

Les variables en Prysma sont déclarées avec un type spécifique suivi du nom et optionnellement d'une valeur initiale.

### Syntaxe:
```prysma
type nomVariable = valeur;
```

### Exemples:
```prysma
int entier = 10;
float flottant = 20.5;
string texte = "Bonjour, Prysma!";
bool booleen = true;
```

---

## Fonctions

Les fonctions en Prysma sont définies avec le mot-clé `def`. Ce choix architectural facilite la logique du lexer et améliore la clarté du code.

### Syntaxe:
```prysma
def typeRetour nomFonction(type param1, type param2, ...) {
    // Corps de la fonction
    return valeur;
}
```

### Exemples:

#### Fonction avec paramètres et retour:
```prysma
def int addition(int a, int b) {
    return a + b;
}
```

#### Fonction sans paramètres:
```prysma
def int obtenir_nombre() {
    return 42;
}
```

#### Fonction avec plusieurs paramètres:
```prysma
def float calculer_moyenne(float note1, float note2, float note3) {
    float somme = note1 + note2 + note3;
    return somme / 3;
}
```

#### Fonction sans retour:
```prysma
def void afficher_message(string message) {
    print(message);
}
```

---

## Structures de contrôle

### Conditionnelle `if...else`

Exécute du code selon une condition booléenne.

```prysma
if (condition) {
    // Code exécuté si la condition est vraie
} else {
    // Code exécuté si la condition est fausse
}
```

**Exemple:**
```prysma
if (age >= 18) {
    print("Vous êtes majeur");
} else {
    print("Vous êtes mineur");
}
```

### Boucle `while`

Répète du code tant qu'une condition est vraie.

```prysma
while (condition) {
    // Code à exécuter tant que la condition est vraie
}
```

**Exemple:**
```prysma
int compteur = 0;
while (compteur < 10) {
    print(compteur);
    compteur = compteur + 1;
}
```

### Boucle `for`

Itère sur une plage de valeurs.

```prysma
for (initialisation; condition; incrémentation) {
    // Code à exécuter à chaque itération
}
```

**Exemple:**
```prysma
for (int i = 0; i < 10; i = i + 1) {
    if (i % 2 == 0) {
        print("Pair: " + i);
    } else {
        print("Impair: " + i);
    }
}
```

```pryma

def bool determiner_nombre_pair(int nombre) {
    int result = false; 
    for (int i = 0; i < nombre; i = i + 1) {
        if (i % 2 == 0) {
            result = true;
            print("Pair: " + i);
        }
        else 
        {
            result = false;
            print("Impair: " + i);
        }
    }
    return result;
}

```

---

## Commentaires



### Commentaires sur une seule ligne
```prysma
// Ceci est un commentaire sur une seule ligne
int x = 5;  // Ce commentaire explique la ligne
```
## Opérateurs

### Opérateurs arithmétiques

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `+` | Addition | `a + b` |
| `-` | Soustraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |

### Opérateurs de comparaison

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `==` | Égal à | `a == b` |
| `!=` | Différent de | `a != b` |
| `<` | Inférieur à | `a < b` |
| `>` | Supérieur à | `a > b` |
| `<=` | Inférieur ou égal à | `a <= b` |
| `>=` | Supérieur ou égal à | `a >= b` |

### Opérateur d'assignation

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `=` | Assignation | `a = 5` |

---

## Notes supplémentaires

### Convention de nommage recommandée

- **Variables et fonctions:** `camelCase` ou `snake_case`
  - Exemples: `nombreEntier`, `nom_utilisateur`, `calculerMoyenne`
- **Constantes:** `UPPER_SNAKE_CASE`
  - Exemples: `MAX_ITERATIONS`, `DEFAULT_VALUE`

