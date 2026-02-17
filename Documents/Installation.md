# Des explications de configuration sont spécifique à la version bleu de VsCode 

## 1. Prérequis système

### Installation de LLVM
```bash
sudo apt install llvm-dev llvm
```

### Installation de Bear (pour la génération de compile_commands.json)
```bash
sudo apt-get update && sudo apt-get install -y bear
```

### Installation de Graphviz (pour la visualisation des graphes)
```bash
sudo apt install graphviz
```

## 2. Configuration du projet

### Créer le répertoire de compilation
```bash
cmake -S . -B build
```

### Générer le fichier compile_commands.json
Ce fichier permet à **clangd** d'analyser correctement votre code.

```bash
mkdir -p build && bear --output build/compile_commands.json -- make -j$(nproc)
```

## 3. Visualisation des graphes

Pour convertir un graphe au format PNG :
```bash
dot -Tpng mon_graphe.dot -o mon_graphe.png
```

## 4. Configuration de VS Code (tasks.json)

Incluez cette tâche dans votre `tasks.json` pour activer la vérification des erreurs lors de `Ctrl+Shift+B` :

```json
{
  "label": "Compiler Prysma (Build)",
  "type": "shell",
  "command": "./Prysma",
  "args": ["${file}"],
  "options": {
    "cwd": "${workspaceFolder}/build"
  },
  "group": {
    "kind": "build",
    "isDefault": true
  },
  "presentation": {
    "reveal": "silent",
    "panel": "shared",
    "clear": true,
    "focus": false
  },
  "problemMatcher": {
    "owner": "prysma_language",
    "fileLocation": ["absolute"],
    "pattern": {
      "regexp": "^([^:]+):(\\d+):(\\d+):\\s+(?:Erreur|error|warning):\\s+(.*)$",
      "file": 1,
      "line": 2,
      "column": 3,
      "message": 4
    }
  }
}
```

Cette configuration permet à VS Code de :
- Souligner les erreurs et avertissements dans l'éditeur
- Naviguer rapidement vers les erreurs
- Afficher les messages diagnostiques pertinents

## 5. Configuration de la coloration syntaxique Prysma

### Installation des prérequis Node.js
Vous devez avoir Node.js installé pour générer la grammaire de coloration syntaxique.

```bash
sudo apt install nodejs npm
```

### Compiler la grammaire TextMate (prysma.tmLanguage.json)
Naviguez vers le répertoire contenant le fichier de grammaire et compilez-le :

```bash
cd ${workspaceFolder}/.vscode/SyntaxeCouleurPrysma
npm install -g vsce
vsce package
```
Dans vscode bleu click droit sur le fichier généré clicker en bas sur `Install Extension VSIX`

### Appliquer la coloration aux fichiers .p
Une fois compilée, la grammaire Prysma s'applique **automatiquement** à tous les fichiers avec l'extension `.p`.

Pour vérifier que la coloration fonctionne :
1. Ouvrez un fichier `.p` dans VS Code
2. Les éléments du langage devraient s'afficher avec des couleurs
3. Si nécessaire, rechargez la fenêtre : `Ctrl+Shift+P` → "Developer: Reload Window"
