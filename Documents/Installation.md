sudo apt install llvm-dev llvm

cmake -S . -B build

sudo apt-get update && sudo apt-get install -y bear


# Construire le fichier compile_commands.json permetant une analyse correct du code par clangd
mkdir -p build && bear --output build/compile_commands.json -- make