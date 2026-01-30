# Faire un graphe
## faire un fichier dot 
opt -passes=dot-cfg -disable-output output.ll

## Traduire en photo 
dot -Tpng .main.dot -o main.png


 clang -g -O0 -fsanitize=address output.ll -o debug_bin