import os
import shutil
from Tests.Orchestration.build_manager import BuildManager
from generation.generateur_ast import GenerateurAST
from generation.generateur_interface_visiteur import GenerateurInterfaceVisiteur
from generation.generateur_visiteur_base_generale import GenerateurVisiteurBaseGenerale
from generation.generateur_graphe_viz import GenerateurGrapheViz
from generation.generateur_expression import GenerateurExpression
from generation.generateur_parseur import GenerateurParseur


def main():
    dossier_script = os.path.dirname(os.path.abspath(__file__))
    os.chdir(dossier_script)

    GenerateurAST(dossier_script).generer()
    GenerateurInterfaceVisiteur(dossier_script).generer()
    GenerateurVisiteurBaseGenerale(dossier_script).generer()
    GenerateurGrapheViz(dossier_script).generer()
    GenerateurExpression(dossier_script).generer()
    GenerateurParseur(dossier_script).generer()

    BuildManager.executer_commande(["cmake", "-S", ".", "-B", "build"])
    os.makedirs("build", exist_ok=True)
    shutil.rmtree(os.path.join("build", "obj"), ignore_errors=True)

    nb_coeurs = str(os.cpu_count() or 1)
    cxxflags = "-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wnull-dereference -Wformat=2 -Werror"

    BuildManager.executer_commande([
        "bear", "--output", "build/compile_commands.json", "--", 
        "make", f"-j{nb_coeurs}", 
        f"CXXFLAGS={cxxflags}"
    ])
    
if __name__ == "__main__":
    main()