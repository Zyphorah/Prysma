import os
import shutil
from Tests.Orchestration.build_manager import BuildManager
from generation.generateur_ast import GenerateurAST
from generation.generateur_interface_visiteur import GenerateurInterfaceVisiteur
from generation.generateur_visiteur_base_generale import GenerateurVisiteurBaseGenerale
from generation.generateur_graphe_viz import GenerateurGrapheViz
from generation.generateur_expression import GenerateurExpression
from generation.generateur_parseur import GenerateurParseur
from generation.generateur_visiteur_destruction import GenerateurVisiteurDestruction


def main():
    dossier_script = os.path.dirname(os.path.abspath(__file__))
    os.chdir(dossier_script)

    GenerateurAST(dossier_script).generer()
    GenerateurInterfaceVisiteur(dossier_script).generer()
    GenerateurVisiteurBaseGenerale(dossier_script).generer()
    GenerateurGrapheViz(dossier_script).generer()
    GenerateurExpression(dossier_script).generer()
    GenerateurParseur(dossier_script).generer()
    GenerateurVisiteurDestruction(dossier_script).generer()

    cxxflags = (
        "-fno-rtti -g3 -O0 " # Debug info max, aucune optimisation
        "-fsanitize=address -fsanitize=undefined " # Les détecteurs de bugs mémoire
        "-fstack-protector-all " # Ajout du protecteur de pile
        "-fno-omit-frame-pointer " # Pour des backtraces propres
        "-D_DEBUG " # Assertions et debug de la lib standard
        "-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor "
        "-Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual "
        "-Wconversion -Wsign-conversion -Wnull-dereference -Wformat=2 "
        "-ffunction-sections -fdata-sections " # section pour le code mort 
        "-Werror "
    )

    # 2. Les sanitizers doivent aussi être passés au Linker
    ldflags = "-fsanitize=address -fsanitize=undefined"

    BuildManager.executer_commande([
        "cmake", "-S", ".", "-B", "build", 
        "-DCMAKE_BUILD_TYPE=Debug",
        f"-DCMAKE_CXX_FLAGS={cxxflags}",
        f"-DCMAKE_EXE_LINKER_FLAGS={ldflags}", 
        f"-DCMAKE_MODULE_LINKER_FLAGS={ldflags}",
        f"-DCMAKE_SHARED_LINKER_FLAGS={ldflags}",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
    ])

    os.makedirs("build", exist_ok=True)
    shutil.rmtree(os.path.join("build", "obj"), ignore_errors=True)

    nb_coeurs = str(os.cpu_count() or 1)

    BuildManager.executer_commande([
        "cmake", "--build", "build", "--parallel", nb_coeurs
    ])
    
if __name__ == "__main__":
    main()