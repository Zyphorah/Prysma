import subprocess
import os

class TestCppUnittest:
    
    def __init__(self, build_dirname="build", test_executable="PrysmaTests"):
        
        # Chemins relatifs à l'emplacement du script
        self.script_dir = os.path.dirname(os.path.abspath(__file__))
        self.project_root = os.path.abspath(os.path.join(self.script_dir, "../../"))
        self.build_dir = os.path.join(self.project_root, "Tests", build_dirname)
        self.test_executable_path = os.path.join(self.build_dir, test_executable)

    def executer_tests(self):  

        if not os.path.exists(self.test_executable_path):
            print(f" Error: L'exécutable de test n'existe pas: {self.test_executable_path}")
            return False
        
        try:
            result = subprocess.run(
                [self.test_executable_path],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            self.__afficher_resultats(result)
            return result.returncode == 0
            
        except Exception as e:
            print(f" Error lors de l'exécution des tests: {str(e)}")
            return False

    def __afficher_resultats(self, result):

        # Code couleur ANSI
        VERT = "\033[92m"
        ROUGE = "\033[91m"
        RESET = "\033[0m"
        BOLD = "\033[1m"
        
        if result.returncode == 0:
            print(f"{VERT}{BOLD}Tests unitaires C++ réussis{RESET}")
        else:
            print(f"{ROUGE}{BOLD}Certains tests ont échoué{RESET}")
            print(f"{ROUGE}Code de return: {result.returncode}{RESET}")
            
            if result.stderr:
                print(f"\n{ROUGE}Détails des errors:{RESET}")
                # Afficher les errors ligne par ligne
                for ligne in result.stderr.split('\n'):
                    if ligne.strip():
                        print(f"{ROUGE}{ligne}{RESET}")
            
            if result.stdout:
                print(f"\n{ROUGE}Output du test:{RESET}")
                # Afficher les résultats des tests échoués
                for ligne in result.stdout.split('\n'):
                    if 'FAILED' in ligne or 'Error' in ligne or 'error' in ligne:
                        print(f"{ROUGE}{ligne}{RESET}")
                    elif ligne.strip():
                        print(ligne)
