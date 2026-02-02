cmake_minimum_required(VERSION 3.10)
project(Prysma)

# Configuration du standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Générer compile_commands.json pour clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Par défaut, si aucun type de build n'est fourni, utiliser Debug
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE non défini -> configuration par défaut: Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# --- BLOC LLVM AJOUTÉ ---
# 1. Localisation du paquet LLVM
find_package(LLVM REQUIRED CONFIG)

# 2. Définition des macros de préprocesseur LLVM
add_definitions(${LLVM_DEFINITIONS})

# 3. Sélection des composants (Core et Support sont le minimum)
# Ajoutez les architectures cibles pour InitializeAllTargets()
llvm_map_components_to_libnames(llvm_libs 
    core 
    support 
    irreader 
    bitwriter 
    ${LLVM_TARGETS_TO_BUILD}
)
# ------------------------

# Dossiers include (Ajout de ${LLVM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include ${LLVM_INCLUDE_DIRS})

# Sources
set(SOURCES
    src/main.cpp
    src/Compilateur/AST/ConstructeurArbreEquation.cpp
    src/Compilateur/AST/GestionnaireChargementVariable.cpp
    src/Compilateur/AST/Noeuds/Operande/Operation.cpp
    src/Compilateur/AST/Noeuds/Operande/Valeur.cpp
    src/Compilateur/AST/Noeuds/Operande/NoeudVariable.cpp
    src/Compilateur/AST/Noeuds/Variable/NoeudDeclaration.cpp
    src/Compilateur/AST/Noeuds/Variable/NoeudAffectation.cpp
    src/Compilateur/LLVM/LLVMSerializer.cpp
    src/Compilateur/LLVM/LLVMBackend.cpp
    src/Compilateur/Parsing/Equation/ChaineResponsabilite.cpp
    src/Compilateur/Parsing/Equation/GestionnaireOperateur.cpp
    src/Compilateur/Parsing/Equation/ServiceParenthese.cpp
    src/Compilateur/Lexer/Lexer.cpp
    src/Compilateur/Builder/Equation/FloatEquationBuilder.cpp
    src/Compilateur/Builder/Equation/IntEquationBuilder.cpp
    src/Compilateur/TraitementFichier/FichierLecture.cpp
    src/Compilateur/TraitementFichier/FichierEcriture.cpp
    src/Compilateur/AST/Registre/RegistreVariable.cpp
    src/Compilateur/AST/ConstructeurArbreInstruction.cpp
    src/Compilateur/Parsing/Instruction/ParseurMain.cpp
    src/Compilateur/Parsing/Instruction/Variable/ParseurDeclaration.cpp
    src/Compilateur/Parsing/Instruction/Variable/ParseurAffectation.cpp
    src/Compilateur/Parsing/Equation/ParseurEquation.cpp
    src/Compilateur/Parsing/ParserBase.cpp
    src/Compilateur/Parsing/Instruction/Fonction/ParserDeclarationFonction.cpp
    src/Compilateur/AST/Noeuds/NoeudMain.cpp
    src/Compilateur/AST/Noeuds/Instruction.cpp
    src/Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.cpp
)

# Créer l'exécutable
add_executable(Prysma ${SOURCES})

# --- LIEN LLVM AJOUTÉ ---
# Lier les bibliothèques LLVM identifiées plus haut
target_link_libraries(Prysma PRIVATE ${llvm_libs})
# ------------------------

# Flags de compilation
if(MSVC)
    target_compile_options(Prysma PRIVATE /W4)
else()
endif()

# Options de compilation par configuration (ajout des symboles de debug en Debug)
target_compile_options(Prysma PRIVATE
    $<$<CONFIG:Debug>:-g;-O0;-fno-omit-frame-pointer>
    $<$<NOT:$<CONFIG:Debug>>:-O2>
    -Wall -Wextra -Wpedantic
)