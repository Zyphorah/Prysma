# NOTE: Some configuration explanations are specific to the blue version of VSCode

## 1. System Prerequisites

### Installing LLVM
```bash
sudo apt install llvm-dev llvm
```

### Installing Bear (for generating compile_commands.json)
```bash
sudo apt-get update && sudo apt-get install -y bear
```

### Installing Graphviz (for graph visualization)
```bash
sudo apt install graphviz
```

## Installing lld for final compilation

```bash
sudo apt install lld
```

## 2. Python Virtual Environment

```bash
python3 -m venv .venv
source .venv/bin/activate
```

## 3. Python Prerequisites
Make sure you have Python 3 installed, as well as the necessary dependencies for the project

```bash
uv pip install -r requirements.txt
```

## 4. Project Configuration

### Project Compilation
 ```bash
 python3 build.py
 ```
 
## 5. VS Code Configuration (tasks.json)

Include this task in your `tasks.json` to enable error checking with `Ctrl+Shift+B`:

```json
{
  "label": "Compile Prysma (Build)",
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

This configuration allows VS Code to:
- Highlight errors and warnings in the editor
- Quickly navigate to errors
- Display relevant diagnostic messages

## 6. Prysma Syntax Highlighting Configuration

### Installing Node.js Prerequisites
You need to have Node.js installed to generate the syntax highlighting grammar.

```bash
sudo apt install nodejs npm
```

### Compile the TextMate Grammar (prysma.tmLanguage.json)
Navigate to the directory containing the grammar file and compile it:

```bash
cd ${workspaceFolder}/.vscode/SyntaxeCouleurPrysma
npm install -g vsce
vsce package
```
In blue VSCode, right-click on the generated file and click at the bottom on `Install Extension VSIX`

### Apply Highlighting to .p Files
Once compiled, the Prysma grammar is applied **automatically** to all files with the `.p` extension.

To check that highlighting works:
1. Open a `.p` file in VS Code
2. Language elements should appear with colors
3. If necessary, reload the window: `Ctrl+Shift+P` → "Developer: Reload Window"
