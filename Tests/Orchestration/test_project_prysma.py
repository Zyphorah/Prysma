import subprocess
import os

class TestProjectPrysma:
    
    def __init__(self, build_dirname, compiler_name, executable_name, file):
        # Chemins relatifs à l'emplacement du script
        self.script_dir = os.path.dirname(os.path.abspath(__file__))
        self.project_root = os.path.abspath(os.path.join(self.script_dir, "../../"))
        self.build_dir = os.path.join(self.project_root, build_dirname)
        self.compiler_path = os.path.join(self.build_dir, compiler_name)
        self.executable_path = os.path.join(self.build_dir, executable_name)
        self.file  = file 

    def __get_false_lines(self, result):
        """Récupère toutes les lignes contenant 'false'"""
        return [ligne for ligne in result.stdout.split('\n') if 'false' in ligne.lower()]

        
    def executer_project(self):
            
        print("--- Lancement des tests (seuls les échecs sont affichés) ---")

        file_path = os.path.join(self.project_root, self.file)
        
        # Compiler
        command = [self.compiler_path, file_path]
        compile_result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        if compile_result.returncode != 0:
            return self.__afficher_resultats(compile_result, command)
        
        # Exécuter l'exécutable généré (executable_path)
        if not os.path.exists(self.executable_path):
            print(f"Exécutable non trouvé: {self.executable_path}")
            return False
        
        exec_command = [self.executable_path]
        result = subprocess.run(exec_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        return self.__afficher_resultats(result, exec_command)

    def __afficher_resultats(self, result, command):
        
        VERT = "\033[92m"
        ROUGE = "\033[91m"
        RESET = "\033[0m"
        BOLD = "\033[1m"

        if result.returncode != 0:
            print(f"{ROUGE}{BOLD}Error lors de l'exécution{RESET}")
            print(f"{ROUGE}Commande: {' '.join(command)}{RESET}")
            print(f"{ROUGE}Code de return: {result.returncode}{RESET}")
            if result.stderr:
                print(f"{ROUGE}Error: {result.stderr}{RESET}")
            return False
        else:
            false_lines = self.__get_false_lines(result)
            if false_lines:
                print(f"{ROUGE}{BOLD}Test échoué{RESET}")
                for ligne in false_lines:
                    print(f"{ROUGE}Échec: {ligne.strip()}{RESET}")
                return False
            else:
                print(f"{VERT}{BOLD}Test réussi{RESET}")
                return True
