cmake_minimum_required(VERSION 3.12)

# faut absolument forcer clang ICI avant le project() 
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

project(Prysma)

# setup du C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# pour que mon éditeur comprenne le code et souligne les errors direct 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# par défaut on compile en Debug pour voir ce qu'on fait
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE non défini -> configuration par défaut: Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# LLVM 
# on laisse cmake se débrouiller pour trouver llvm et ses paths tout seul
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs 
    core 
    support 
    irreader 
    bitwriter 
    ${LLVM_TARGETS_TO_BUILD}
)

# mes fichiers source 
# on récupère tous mes .cpp (le CONFIGURE_DEPENDS c'est pour plus devoir raser le dossier build quand j'ajoute un fichier)
file(GLOB_RECURSE SOURCES_LOCALES CONFIGURE_DEPENDS "Src/*.cpp")
file(GLOB_RECURSE SOURCES_GENEREES CONFIGURE_DEPENDS "build/generationCode/src/*.cpp")

# on fusionne tout ça 
set(SOURCES ${SOURCES_LOCALES} ${SOURCES_GENEREES})

# au cas où le main.cpp passe à la trappe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp")
    list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Src/main.cpp")
endif()

# on crée l'exécutable
add_executable(Prysma ${SOURCES})

# on balance les définitions llvm
target_compile_definitions(Prysma PRIVATE ${LLVM_DEFINITIONS})

# mes dossiers include
target_include_directories(Prysma PRIVATE 
    ${CMAKE_SOURCE_DIR}/Include 
    ${CMAKE_SOURCE_DIR}/build/generationCode/include
)

# on inclut llvm en SYSTEM pour que clang arrête de crier sur leurs 1000 warnings
target_include_directories(Prysma SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})

# on lie les libs llvm
target_link_libraries(Prysma PRIVATE ${llvm_libs})

# mes flags de compilation
target_compile_options(Prysma PRIVATE
    # pas d'opti en Debug, max d'opti en Release
    $<$<CONFIG:Debug>:-g;-O0;-fno-omit-frame-pointer;-fstandalone-debug>
    $<$<NOT:$<CONFIG:Debug>>:-O2>
    
    # pour lire les errors sans avoir mal au crâne (couleurs, 3 errors max, petit curseur)
    -fcolor-diagnostics     
    -ferror-limit=3        
    -fshow-column            
    -fcaret-diagnostics    
    
    # analyse stricte pour coder proprement
    -Wall -Wextra -Wpedantic 
    -Wshadow                
    -Wnon-virtual-dtor      
    -Wnull-dereference      
)