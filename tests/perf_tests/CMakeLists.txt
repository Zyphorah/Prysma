# Version 3.14 minimum (pour que CMake détecte les nouveaux fichiers automatiquement et utilise FetchContent)
cmake_minimum_required(VERSION 3.14)

# Définition de Clang comme compiler AVANT la déclaration du project
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

project(PrysmaPerfTests)

# Configuration du standard C++17 et de l'exportation pour clangd
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pour les tests de performance, le mode Release est primordial
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Configuration LLVM 
find_package(LLVM 18.1.3 REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs 
    core 
    support 
    irreader 
    bitwriter 
    ${LLVM_TARGETS_TO_BUILD}
)

# Configuration de Google Test via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Configuration de Google Benchmark (souvent utilisé en complément pour la performance)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Gestion automatique des fichiers sources 

# 1. Récupération des sources du project principal
file(GLOB_RECURSE SOURCES_MAIN CONFIGURE_DEPENDS "../../src/*.cpp")

# IMPORTANT : Exclusion du main.cpp du project principal pour éviter les conflits (double function main)
list(REMOVE_ITEM SOURCES_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/../../src/main.cpp")

# 2. Récupération du code généré
file(GLOB_RECURSE SOURCES_GENEREES CONFIGURE_DEPENDS "../../build/generationCode/src/*.cpp")

# 3. Récupération des fichiers de tests locaux (tests de perf)
file(GLOB_RECURSE SOURCES_TESTS CONFIGURE_DEPENDS "src/*.cpp")

# Fusion des listes de sources
set(SOURCES ${SOURCES_MAIN} ${SOURCES_GENEREES} ${SOURCES_TESTS})

# Ajout du point d'entrée des tests s'il est à la racine
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
endif()

# Création de l'exécutable de test
add_executable(PrysmaPerfTests ${SOURCES})


# Configuration de la cible (PrysmaPerfTests) 

target_compile_definitions(PrysmaPerfTests PRIVATE ${LLVM_DEFINITIONS})

# Définition des dossiers d'en-tête (include)
target_include_directories(PrysmaPerfTests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/generationCode/include
)

# Inclusion de LLVM en mode SYSTEM pour masquer ses avertissements internes
target_include_directories(PrysmaPerfTests SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})

# Édition des liens avec les bibliothèques LLVM, Google Test et Google Benchmark
target_link_libraries(PrysmaPerfTests PRIVATE 
    ${llvm_libs}
    gtest_main
    benchmark::benchmark
)

# Options de compilation strictes et formatage des diagnostics
# L'optimisation maximale (-O3) est favorisée hors debug pour les performances
target_compile_options(PrysmaPerfTests PRIVATE
    $<$<CONFIG:Debug>:-g;-O0;-fno-omit-frame-pointer>
    $<$<NOT:$<CONFIG:Debug>>:-O3>
    -fcolor-diagnostics     
    -ferror-limit=3        
    -fshow-column            
    -fcaret-diagnostics    
    -Wall -Wextra -Wpedantic 
    -Wshadow                
    -Wnon-virtual-dtor      
    -Wnull-dereference      
)
