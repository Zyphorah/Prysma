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
    # AnalyseSyntaxique 
    src/Compilateur/AnalyseSyntaxique/Equation/ChaineResponsabilite.cpp
    src/Compilateur/AnalyseSyntaxique/Equation/GestionnaireOperateur.cpp
    src/Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.cpp
    src/Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.cpp
    src/Compilateur/AnalyseSyntaxique/Equation/ServiceParenthese.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.cpp
    src/Compilateur/AnalyseSyntaxique/ParseurBase.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.cpp
    src/Compilateur/AnalyseSyntaxique/Instruction/Boucle/ParseurWhile.cpp
    # ConstructeurArbre
    src/Compilateur/AST/ConstructeurArbreEquation.cpp
    src/Compilateur/AST/ConstructeurArbreInstruction.cpp
    # Noeuds
    src/Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.cpp
    src/Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.cpp
    src/Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.cpp
    src/Compilateur/AST/Noeuds/Fonction/NoeudRetour.cpp
    src/Compilateur/AST/Noeuds/NoeudInstruction.cpp
    src/Compilateur/AST/Noeuds/NoeudScope.cpp
    src/Compilateur/AST/Noeuds/Equation/NoeudOperation.cpp
    src/Compilateur/AST/Noeuds/Operande/NoeudLitteral.cpp
    src/Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.cpp
    src/Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.cpp
    src/Compilateur/AST/Noeuds/Variable/NoeudRefVariable.cpp
    src/Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.cpp
    src/Compilateur/AST/Noeuds/Condition/NoeudIf.cpp
    src/Compilateur/AST/Noeuds/Boucle/NoeudWhile.cpp
    # Arbre syntaxique abstrait
    src/Compilateur/AST/Registre/RegistreArgument.cpp
    src/Compilateur/AST/Registre/RegistreVariable.cpp
    src/Compilateur/AST/Registre/ReturnContextCompilation.cpp
    src/Compilateur/Builder/Equation/ConstructeurEquationFlottante.cpp
    src/Compilateur/Lexer/Lexer.cpp
    src/Compilateur/LLVM/LlvmBackend.cpp
    src/Compilateur/LLVM/LlvmSerializer.cpp
    src/Compilateur/LLVM/GestionFonction.cpp
    src/Compilateur/LLVM/GestionVariable.cpp
    src/Compilateur/TraitementFichier/ConstructeurSysteme.cpp
    src/Compilateur/TraitementFichier/FichierEcriture.cpp
    src/Compilateur/TraitementFichier/FichierLecture.cpp
    src/Compilateur/Visiteur/Fonction/VisiteurAppelFonction.cpp
    src/Compilateur/Visiteur/Boucle/VisiteurNoeudWhile.cpp
    # Visiteur 
    src/Compilateur/Visiteur/Fonction/VisiteurArgFonction.cpp
    src/Compilateur/Visiteur/Fonction/VisiteurDeclarationFonction.cpp
    src/Compilateur/Visiteur/Fonction/VisiteurRetour.cpp
    src/Compilateur/Visiteur/Operande/VisiteurOperation.cpp
    src/Compilateur/Visiteur/Variable/VisiteurAffectationVariable.cpp
    src/Compilateur/Visiteur/Variable/VisiteurDeclarationVariable.cpp
    src/Compilateur/Visiteur/Variable/VisiteurRefVariable.cpp
    src/Compilateur/Visiteur/Variable/VisiteurUnRefVariable.cpp
    src/Compilateur/Visiteur/VisiteurGeneralGenCode.cpp
    src/Compilateur/Visiteur/VisiteurInstruction.cpp
    src/Compilateur/Visiteur/VisiteurScope.cpp
    src/Compilateur/Visiteur/Condition/VisiteurNoeudIf.cpp
    src/Compilateur/Visiteur/Boucle/VisiteurNoeudWhile.cpp
    src/main.cpp
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