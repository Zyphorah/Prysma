import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from Tests.Orchestration import build_manager
from Generation.generator_ast import GenerateurAST
from Generation.generator_interface_visitor import GenerateurInterfaceVisitor
from Generation.generator_visitor_base_general import GenerateurVisitorBaseGenerale
from Generation.generator_graphe_viz import GenerateurGraphViz
import Orchestration.test_project_prysma
import Orchestration.test_cpp_unittest

def main():

    array_tests_prysma = []
    racine = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    GenerateurAST(racine).generatedr()
    GenerateurInterfaceVisitor(racine).generatedr()
    GenerateurVisitorBaseGenerale(racine).generatedr()
    GenerateurGraphViz(racine).generatedr()

    build_manager.BuildManager.executer_commande(["python3", "../debug.py"])

    # Compilation des tests 
    build_manager.BuildManager.executer_commande(["cmake", "-S", ".", "-B", "build"])
    build_manager.BuildManager.executer_commande(["cmake", "--build", "build", "-j", str(os.cpu_count() or 1)])

    test_cpp = Orchestration.test_cpp_unittest.TestCppUnittest()
    ok_cpp = test_cpp.executer_tests()

    # Test 1: ProjectTestEdgeCase
    print("ProjectTestEdgeCase/main.p")
    test_prysma_1 = Orchestration.test_project_prysma.TestProjectPrysma(
        build_dirname="build", 
        compiler_name="Prysma", 
        executable_name="main", 
        file="Tests/PrysmaCodeTests/ProjectTestEdgeCase/main.p"
    )
    array_tests_prysma.append(test_prysma_1.executer_project())

    # Test 2: ProjectTestMultiThreadEdgeCaseFile - TestInclude
    print("\nProjectTestMultiThreadEdgeCaseFile/TestInclude/main.p")
    test_prysma_2 = Orchestration.test_project_prysma.TestProjectPrysma(
        build_dirname="build", 
        compiler_name="Prysma", 
        executable_name="main", 
        file="Tests/PrysmaCodeTests/ProjectTestMultiThreadEdgeCaseFile/TestInclude/main.p"
    )
    array_tests_prysma.append(test_prysma_2.executer_project())

    # Test 3: ProjectTestMultiThreadEdgeCaseFile - TestDepth
    print("\nProjectTestMultiThreadEdgeCaseFile/TestDepth/main.p")
    test_prysma_3 = Orchestration.test_project_prysma.TestProjectPrysma(
        build_dirname="build", 
        compiler_name="Prysma", 
        executable_name="main", 
        file="Tests/PrysmaCodeTests/ProjectTestMultiThreadEdgeCaseFile/TestDepth/main.p"
    )
    array_tests_prysma.append(test_prysma_3.executer_project())

    all_ok = ok_cpp and all(array_tests_prysma)
    
    if not all_ok:
        print("\nArrêt du pipeline : Échec d'une étape.")
        sys.exit(1)

    print("\nTests terminés")
    sys.exit(0)

if __name__ == "__main__":
    main()