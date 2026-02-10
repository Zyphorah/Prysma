## Installation de LLVM 
sudo apt install llvm-dev llvm

## Commande pour faire le fichier build de compilation 
cmake -S . -B build
sudo apt-get update && sudo apt-get install -y bear

## Construire le fichier compile_commands.json permetant une analyse correct du code par clangd
mkdir -p build && bear --output build/compile_commands.json -- make -j$(nproc)

## Installation de graphviz 
sudo apt install graphviz

## utilisation 

dot -Tpng mon_graphe.dot -o mon_graphe.png


## Fichier tasks.json vous devez inclure cette configuration pour avoir les erreurs surligner lorsque vous faite control shift b
{
"label": "Compiler Prysma (Check)",
"type": "shell",

"command": "${workspaceFolder}/build/Prysma",
"args": ["${file}"],

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

    "fileLocation": ["relative", "${workspaceFolder}"],
    "pattern": {
        "regexp": "^(.*):(\\d+):(\\d+):\\s+(?:Erreur|error|warning):\\s+(.*)$",
        "file": 1,
        "line": 2,
        "column": 3,
        "message": 4
    }
}