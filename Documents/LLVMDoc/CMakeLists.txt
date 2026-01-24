cmake_minimum_required(VERSION 3.14)
project(LLVM_Labo)

set(CMAKE_CXX_STANDARD 17)

# 1. Trouver le paquet LLVM
find_package(LLVM REQUIRED CONFIG)

# 2. Configurer les chemins d'inclusion
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/../../include)
add_definitions(${LLVM_DEFINITIONS})

# 3. Définir l'exécutable
add_executable(LLVMMethodeDoc LLVMMethodeDoc.cpp ../../src/Compilateur/LLVM/LLVMSerializer.cpp)

# 4. Utiliser directement les flags de llvm-config sans ALL
execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config --ldflags
    OUTPUT_VARIABLE LLVM_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config --libs
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 5. Appliquer tous les flags et libs
target_compile_options(LLVMMethodeDoc PRIVATE ${LLVM_CXX_FLAGS})
target_link_options(LLVMMethodeDoc PRIVATE ${LLVM_LDFLAGS})
target_link_libraries(LLVMMethodeDoc ${LLVM_LIBS})

# Message de confirmation
message(STATUS "Projet LLVMMethodeDoc configuré avec succès")
message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM dir: ${LLVM_DIR}")
