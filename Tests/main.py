# Execution séquentielle des tests du compilateur
# C'est l'orchestrateur qui lance tous les tests du compilateur
# Orchestrateur de tests pour le compilateur prysma, automatisation des tests unitaire 
import subprocess
import Orchestration.test_projet_prysma
import Orchestration.test_cpp_unittest
import Orchestration.build_manager
import sys

def main():
    tableau_tests_prysma = []

    builder = Orchestration.build_manager.BuildManager(verbose=False)
    
    ok_build_compilateur = builder.build_compilateur()
    ok_build_tests = builder.build_tests()

    test_cpp = Orchestration.test_cpp_unittest.TestCppUnittest()
    ok_cpp = test_cpp.executer_tests()

    # Test 1: ProjetTestCasLimite
    print("ProjetTestCasLimite/main.p")
    test_prysma_1 = Orchestration.test_projet_prysma.TestProjetPrysma(
        build_dirname="build", 
        compiler_name="Prysma", 
        executable_name="main", 
        file="Tests/PrysmaCodeTests/ProjetTestCasLimite/main.p"
    )
    tableau_tests_prysma.append(test_prysma_1.executer_projet())

    # Test 2: ProjetTestMultiThreadCasLimiteFichier - TestInclude
    print("\nProjetTestMultiThreadCasLimiteFichier/TestInclude/main.p")
    test_prysma_2 = Orchestration.test_projet_prysma.TestProjetPrysma(
        build_dirname="build", 
        compiler_name="Prysma", 
        executable_name="main", 
        file="Tests/PrysmaCodeTests/ProjetTestMultiThreadCasLimiteFichier/TestInclude/main.p"
    )
    tableau_tests_prysma.append(test_prysma_2.executer_projet())

    # Test 3: ProjetTestMultiThreadCasLimiteFichier - TestProfondeur
    print("\nProjetTestMultiThreadCasLimiteFichier/TestProfondeur/main.p")
    test_prysma_3 = Orchestration.test_projet_prysma.TestProjetPrysma(
        build_dirname="build", 
        compiler_name="Prysma", 
        executable_name="main", 
        file="Tests/PrysmaCodeTests/ProjetTestMultiThreadCasLimiteFichier/TestProfondeur/main.p"
    )
    tableau_tests_prysma.append(test_prysma_3.executer_projet())

    all_ok = ok_build_compilateur and ok_build_tests and ok_cpp and all(tableau_tests_prysma)
    
    if not all_ok:
        print("\nArrêt du pipeline : Échec d'une étape.")
        sys.exit(1)

    print("\nTests terminés")
    sys.exit(0)

if __name__ == "__main__":
    main()