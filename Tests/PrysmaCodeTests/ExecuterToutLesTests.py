import subprocess
import os

# Chemins relatifs à l'emplacement du script
script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(script_dir, "../../"))
build_dir = os.path.join(project_root, "build")
compiler_path = os.path.join(build_dir, "Prysma")
executable_path = os.path.join(build_dir, "programme")

test_files = [
    "TestTableauInt.p",
    "TestTableauFloat.p",
    "TestTableauBool.p",
    "TestRecursivité.p",
    "TestFonctionnelVariableInt.p",
    "TestFonctionnelVariableFloat.p",
    "TestFonctionnelVariableBool.p",
    "TestCondition.p",
    "TestBoucle.p",
]

print("--- Lancement des tests (seuls les échecs sont affichés) ---")
tests_echoues = []

for test_file in test_files:
    test_file_path = os.path.join(script_dir, test_file)
    
    # Compilation silencieuse
    with open(os.devnull, 'w') as DEVNULL:
        compile_process = subprocess.run(
            [compiler_path, test_file_path], 
            stdout=DEVNULL, stderr=DEVNULL, cwd=build_dir
        )
    
    if compile_process.returncode != 0:
        tests_echoues.append(f"--- ÉCHEC : {test_file} (Compilation) ---")
        continue

    # Exécution
    run_process = subprocess.run(
        [executable_path],
        capture_output=True, text=True, cwd=build_dir
    )
    
    output_lines = run_process.stdout.strip().split('\n')
    failed_lines = [line for line in output_lines if "false" in line]

    if failed_lines:
        tests_echoues.append(f"--- ÉCHEC : {test_file} ---")
        tests_echoues.extend(failed_lines)
        tests_echoues.append("") 

if not tests_echoues:
    print("\n--- Tous les tests ont réussi ---")
else:
    print("\n--- Résumé des échecs ---")
    for echec in tests_echoues:
        print(echec)

