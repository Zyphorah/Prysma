cmake_minimum_required(VERSION 3.10)
project(Prysma)

# Configuration du standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dossiers include
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sources
set(SOURCES
    src/main.cpp
    src/AST/ConstructeurArbreEquation.cpp
    src/AST/Noeuds/Operande/Operation.cpp
    src/AST/Noeuds/Operande/RegistreSymbole.cpp
    src/AST/Noeuds/Operande/Valeur.cpp
    src/Parsing/Equation/ChaineResponsabilite.cpp
    src/Parsing/Equation/GestionnaireOperateur.cpp
    src/Parsing/Equation/ServiceParenthese.cpp
)

# Créer l'exécutable
add_executable(Prysma ${SOURCES})

# Flags de compilation
if(MSVC)
    target_compile_options(Prysma PRIVATE /W4)
else()
    target_compile_options(Prysma PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Options de débogage
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
