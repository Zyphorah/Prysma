cmake_minimum_required(VERSION 3.10)
project(Prysma)

# Configuration du standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Générer compile_commands.json pour clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include ${CMAKE_SOURCE_DIR}/include)

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
    src/Compilateur/AST/test_ast.cpp
    main.cpp
)

# Sources du projet principal nécessaires pour les tests
set(MAIN_SOURCES
    # AnalyseSyntaxique
    ../src/Compilateur/AnalyseSyntaxique/Equation/ChaineResponsabilite.cpp
    ../src/Compilateur/AnalyseSyntaxique/Equation/GestionnaireOperateur.cpp
    ../src/Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.cpp
    ../src/Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.cpp
    ../src/Compilateur/AnalyseSyntaxique/Equation/ServiceParenthese.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.cpp
    ../src/Compilateur/Registre/RegistreFichier.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.cpp
    ../src/Compilateur/AnalyseSyntaxique/ParseurBase.cpp
    ../src/Compilateur/AnalyseSyntaxique/ParseurType.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Boucle/ParseurWhile.cpp
    ../src/Compilateur/AnalyseSyntaxique/Instruction/Include/ParseurInclude.cpp
    # ConstructeurArbre
    ../src/Compilateur/AST/ConstructeurArbreEquation.cpp
    ../src/Compilateur/AST/ConstructeurArbreInstruction.cpp
    # Noeuds
    ../src/Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.cpp
    ../src/Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.cpp
    ../src/Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.cpp
    ../src/Compilateur/AST/Noeuds/Fonction/NoeudRetour.cpp
    ../src/Compilateur/AST/Noeuds/NoeudInstruction.cpp
    ../src/Compilateur/AST/Noeuds/Equation/NoeudOperation.cpp
    ../src/Compilateur/AST/Noeuds/Operande/NoeudLitteral.cpp
    ../src/Compilateur/AST/Noeuds/Operande/NoeudNegation.cpp
    ../src/Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.cpp
    ../src/Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.cpp
    ../src/Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.cpp
    ../src/Compilateur/AST/Noeuds/Variable/NoeudRefVariable.cpp
    ../src/Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.cpp
    ../src/Compilateur/AST/Noeuds/Condition/NoeudIf.cpp
    ../src/Compilateur/AST/Noeuds/Boucle/NoeudWhile.cpp
    ../src/Compilateur/AST/Noeuds/Include/NoeudInclude.cpp
    # Tableau
    ../src/Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.cpp
    ../src/Compilateur/AST/Noeuds/Tableau/NoeudLectureTableau.cpp
    # StrategieEquation
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieRef.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieUnRef.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieNegation.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieLitteral.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.cpp
    ../src/Compilateur/AST/Noeuds/StrategieEquation/StrategieString.cpp
    # Registres
    ../src/Compilateur/AST/Registre/RegistreArgument.cpp
    ../src/Compilateur/AST/Registre/RegistreVariable.cpp
    ../src/Compilateur/AST/Registre/ReturnContextCompilation.cpp
    ../src/Compilateur/AST/Registre/TypeSimple.cpp
    ../src/Compilateur/AST/Registre/TypeTableau.cpp
    # Builder
    ../src/Compilateur/Builder/Equation/ConstructeurEquationFlottante.cpp
    ../src/Compilateur/Utils/ConstructeurEnvironnementRegistreVariable.cpp
    ../src/Compilateur/Utils/ConstructeurEnvironnementRegistreFonction.cpp
    # Lexer
    ../src/Compilateur/Lexer/Lexer.cpp
    # LLVM
    ../src/Compilateur/LLVM/LlvmBackend.cpp
    ../src/Compilateur/LLVM/LlvmSerializer.cpp
    ../src/Compilateur/LLVM/GestionFonction.cpp
    ../src/Compilateur/LLVM/GestionVariable.cpp
    # TraitementFichier
    ../src/Compilateur/TraitementFichier/ConstructeurSysteme.cpp
    ../src/Compilateur/TraitementFichier/FichierEcriture.cpp
    ../src/Compilateur/TraitementFichier/FichierLecture.cpp
    # Visiteur de base
    ../src/Compilateur/Visiteur/VisiteurBaseGenerale.cpp
    # Visiteur remplissage registre
    ../src/Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageVariable.cpp
    ../src/Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.cpp
    ../src/Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageFonction.cpp
    ../src/Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageInclude.cpp
    # Visiteur code gen
    ../src/Compilateur/Visiteur/CodeGen/Fonction/VisiteurAppelFonction.cpp
    ../src/Compilateur/Visiteur/CodeGen/Boucle/VisiteurNoeudWhile.cpp
    ../src/Compilateur/Visiteur/CodeGen/Fonction/VisiteurArgFonction.cpp
    ../src/Compilateur/Visiteur/CodeGen/Fonction/VisiteurDeclarationFonction.cpp
    ../src/Compilateur/Visiteur/CodeGen/Fonction/VisiteurRetour.cpp
    ../src/Compilateur/Visiteur/CodeGen/Operande/VisiteurOperation.cpp
    ../src/Compilateur/Visiteur/CodeGen/Operande/VisiteurNoeudLitteral.cpp
    ../src/Compilateur/Visiteur/CodeGen/Operande/VisiteurNoeudNegation.cpp
    ../src/Compilateur/Visiteur/CodeGen/Operande/VisiteurTableauInitialisation.cpp
    ../src/Compilateur/Visiteur/CodeGen/Variable/VisiteurAffectationVariable.cpp
    ../src/Compilateur/Visiteur/CodeGen/Variable/VisiteurDeclarationVariable.cpp
    ../src/Compilateur/Visiteur/CodeGen/Variable/VisiteurRefVariable.cpp
    ../src/Compilateur/Visiteur/CodeGen/Variable/VisiteurUnRefVariable.cpp
    ../src/Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.cpp
    ../src/Compilateur/Visiteur/CodeGen/VisiteurInstruction.cpp
    ../src/Compilateur/Visiteur/CodeGen/Condition/VisiteurNoeudIf.cpp
    # Tableau code gen
    ../src/Compilateur/Visiteur/CodeGen/Tableau/VisiteurAffectationTableau.cpp
    ../src/Compilateur/Visiteur/CodeGen/Tableau/VisiteurLectureTableau.cpp
    # Visiteur AST GraphViz
    ../src/Compilateur/Visiteur/ASTGraphViz/Boucle/VisiteurNoeudWhile.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Condition/VisiteurNoeudIf.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Fonction/VisiteurAppelFonction.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Fonction/VisiteurArgFonction.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Fonction/VisiteurDeclarationFonction.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Fonction/VisiteurRetour.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Operande/VisiteurOperation.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Operande/VisiteurNoeudNegation.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Operande/VisiteurTableauInitialisation.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Variable/VisiteurAffectationVariable.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Variable/VisiteurDeclarationVariable.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Variable/VisiteurRefVariable.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Variable/VisiteurUnRefVariable.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralASTGraphViz.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/VisiteurInstruction.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Include/VisiteurNoeudInclude.cpp
    # Tableau graphviz
    ../src/Compilateur/Visiteur/ASTGraphViz/Tableau/VisiteurAffectationTableau.cpp
    ../src/Compilateur/Visiteur/ASTGraphViz/Tableau/VisiteurLectureTableau.cpp
    # Graph visuel
    ../src/Compilateur/GrapheVisuel/SortieArbreVisuelTexte.cpp
    # Orchestrateur include
    ../src/Compilateur/Utils/OrchestrateurInclude/OrchestrateurInclude.cpp
    ../src/Compilateur/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.cpp
    ../src/Compilateur/Utils/OrchestrateurInclude/UniteCompilation.cpp
)

# Créer l'exécutable
add_executable(PrysmaTests
    src/Compilateur/Lexer/test_lexer.cpp
    src/Compilateur/AST/test_ast.cpp
    src/Compilateur/AST/Registre/test_registre_fonction.cpp
    src/Compilateur/AST/Registre/test_registre_globale.cpp
    src/Compilateur/AST/Registre/test_registre_variable.cpp
    ${MAIN_SOURCES}
)

# --- LIEN LLVM AJOUTÉ ---
# Lier les bibliothèques LLVM identifiées plus haut
target_link_libraries(PrysmaTests PRIVATE ${llvm_libs})
# ------------------------


if(MSVC)
    target_compile_options(PrysmaTests PRIVATE /W4)
else()
endif()

