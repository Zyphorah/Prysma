# Faire un graphe
## faire un fichier dot 
opt -passes=dot-cfg -disable-output output.ll

## Traduire en photo 
dot -Tpng .main.dot -o main.png