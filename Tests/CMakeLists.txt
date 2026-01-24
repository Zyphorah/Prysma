cmake_minimum_required(VERSION 3.10)
project(Prysma_Tests)

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

# --- BLOC LLVM ---
find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core support irreader bitwriter)
# ----------------

# Dossiers include
include_directories(${CMAKE_SOURCE_DIR}/../include ${LLVM_INCLUDE_DIRS})

# Sources des tests
set(TEST_SOURCES
    main.cpp
)

# Créer l'exécutable de test
add_executable(Prysma_Tests ${TEST_SOURCES})

# Lier les bibliothèques LLVM
target_link_libraries(Prysma_Tests PRIVATE ${llvm_libs})

# Flags de compilation
target_compile_options(Prysma_Tests PRIVATE
    $<$<CONFIG:Debug>:-g;-O0;-fno-omit-frame-pointer>
    $<$<NOT:$<CONFIG:Debug>>:-O2>
    -Wall -Wextra -Wpedantic
)
