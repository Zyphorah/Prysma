import os

NB_FICHIERS = 2000
DOSSIER = "stress_test"

os.makedirs(DOSSIER, exist_ok=True)

with open(f"{DOSSIER}/main.p", "w") as main_file:
    # Génération des 2000 fichiers et de leurs includes
    for i in range(1, NB_FICHIERS + 1):
        main_file.write(f'include "fichier_{i}.p";\n')
        
        with open(f"{DOSSIER}/fichier_{i}.p", "w") as f:
            f.write(f"fn int64 func_{i}() {{\n    return {i};\n}}\n")

    # Génération de la function principale dans main.p
    main_file.write("\nfn void executerStressTest() {\n")
    main_file.write('    dec string[] msg = "Test 2000 fichiers : ";\n')
    main_file.write('    call print(ref msg);\n')
    
    # Chaînage des 2000 calls
    calls = " + ".join([f"call func_{i}()" for i in range(1, NB_FICHIERS + 1)])
    main_file.write(f"    dec int64 resultat = {calls};\n")
    
    main_file.write("    call print(resultat);\n")
    main_file.write("    call backSlashN();\n")
    main_file.write("}\n\n")

    # Génération du point d'entrée main
    main_file.write("fn int32 main()\n")
    main_file.write("{\n")
    main_file.write("    call executerStressTest();\n")
    main_file.write("    return 0;\n")
    main_file.write("}\n")

print(f"Génération terminée dans '{DOSSIER}'. Résultat attendu : {NB_FICHIERS * (NB_FICHIERS + 1) // 2}")