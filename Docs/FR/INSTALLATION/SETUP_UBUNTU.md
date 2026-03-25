# NOTE : Des explications de configuration sont spécifique à la version bleu de VsCode 

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

## Installation du lld pour la compilation final

```bash
sudo apt install lld
```

## 2. Environnement virtuel Python

```bash
python3 -m venv .venv
source .venv/bin/activate
```

## 3. Prérequis Python
Assurez-vous d'avoir Python 3 installé, ainsi que les dépendances nécessaires pour le project

```bash
uv pip install -r requirements.txt
```

## 4. Configuration du project

### Compilation du project 
 ```bash
 python3 build.py
 ```
 
## 5. Configuration de VS Code (tasks.json)

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
      "regexp": "^([^:]+):(\\d+):(\\d+):\\s+(?:Error|error|warning):\\s+(.*)$",
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
- Naviguer rapidement vers les errors
- Afficher les messages diagnostiques pertinents

## 6. Configuration de la coloration syntaxique Prysma

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

Pour vérifier que la coloration functionne :
1. Ouvrez un fichier `.p` dans VS Code
2. Les éléments du langage devraient s'afficher avec des couleurs
3. Si nécessaire, reloadz la fenêtre : `Ctrl+Shift+P` → "Developer: Reload Window"
