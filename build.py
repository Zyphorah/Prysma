import os
import shutil
from Tests.Orchestration.build_manager import BuildManager
from generation.generateur_ast import GenerateurAST
from generation.generateur_interface_visiteur import GenerateurInterfaceVisiteur
from generation.generateur_visiteur_base_generale import GenerateurVisiteurBaseGenerale
from generation.generateur_visiteur_destruction import GenerateurVisiteurDestruction
from generation.generateur_graphe_viz import GenerateurGrapheViz
from generation.generateur_expression import GenerateurExpression
from generation.generateur_parseur import GenerateurParseur

def main():
    dossier_script = os.path.dirname(os.path.abspath(__file__))
    os.chdir(dossier_script)

    GenerateurAST(dossier_script).generer()
    GenerateurInterfaceVisiteur(dossier_script).generer()
    GenerateurVisiteurBaseGenerale(dossier_script).generer()
    GenerateurVisiteurDestruction(dossier_script).generer()
    GenerateurGrapheViz(dossier_script).generer()
    GenerateurExpression(dossier_script).generer()
    GenerateurParseur(dossier_script).generer()

    cxxflags_list = [
            "-O3",                  # Vitesse brute (optimisation maximale)
            "-march=native",        # Exploitation totale des instructions de ton CPU
            "-ffast-math",          # Calculs mathématiques agressifs
            "-fno-rtti",            # Zéro métadonnée d'exécution (RTTI off)
            "-fomit-frame-pointer", # Libération d'un registre CPU
            "-flto",                # Fusion et optimisation globale au linkage (LTO)
            "-DNDEBUG"              # Désactivation absolue des assertions
    ]
    
    ldflags_list = [
        "-flto",
        "-Wl,--gc-sections",
        "-Wl,-s",
        "-fuse-ld=lld"
    ]

    cxxflags = " ".join(cxxflags_list)
    ldflags = " ".join(ldflags_list)

    BuildManager.executer_commande([
        "cmake", "-S", ".", "-B", "build", 
        "-DCMAKE_BUILD_TYPE=Release",
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