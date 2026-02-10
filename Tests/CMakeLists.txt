cmake_minimum_required(VERSION 3.10)
project(Prysma)

# Configuration du standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Générer compile_commands.json pour clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

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
include_directories(${CMAKE_SOURCE_DIR}/../include ${CMAKE_SOURCE_DIR}/include ${LLVM_INCLUDE_DIRS})

# Sources
set(SOURCES
    ../src/Compilateur/Lexer/Lexer.cpp
    src/Compilateur/Lexer/test_lexer.cpp
    main.cpp
)

# Créer l'exécutable
add_executable(PrysmaTests src/Compilateur/Lexer/test_lexer.cpp ../src/Compilateur/Lexer/Lexer.cpp)

# --- LIEN LLVM AJOUTÉ ---
# Lier les bibliothèques LLVM identifiées plus haut
target_link_libraries(PrysmaTests PRIVATE ${llvm_libs})
# ------------------------


if(MSVC)
    target_compile_options(PrysmaTests PRIVATE /W4)
else()
endif()

target_compile_options(PrysmaTests PRIVATE
    $<$<CONFIG:Debug>:-g;-O0;-fno-omit-frame-pointer>
    $<$<NOT:$<CONFIG:Debug>>:-O2>
    -Wall -Wextra -Wpedantic
)