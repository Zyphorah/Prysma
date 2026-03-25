import os
import shutil
from Tests.Orchestration.build_manager import BuildManager
from Generation.generator_ast import GenerateurAST
from Generation.generator_interface_visitor import GenerateurInterfaceVisitor
from Generation.generator_visitor_base_general import GenerateurVisitorBaseGenerale
from Generation.generator_visitor_destruction import GenerateurVisitorDestruction
from Generation.generator_graphe_viz import GenerateurGraphViz
from Generation.generator_expression import GenerateurExpression
from Generation.generator_parser import GenerateurParser

def main():
    dossier_script = os.path.dirname(os.path.abspath(__file__))
    os.chdir(dossier_script)

    GenerateurAST(dossier_script).generatedr()
    GenerateurInterfaceVisitor(dossier_script).generatedr()
    GenerateurVisitorBaseGenerale(dossier_script).generatedr()
    GenerateurGraphViz(dossier_script).generatedr()
    GenerateurExpression(dossier_script).generatedr()
    GenerateurParser(dossier_script).generatedr()
    GenerateurVisitorDestruction(dossier_script).generatedr()

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
        " "
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