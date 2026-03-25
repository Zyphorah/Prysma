import os

NB_FICHIERS = 32
FONCTIONS_PAR_FICHIER = 12500
DOSSIER = "stress_massif"

os.makedirs(DOSSIER, exist_ok=True)

with open(f"{DOSSIER}/main.p", "w") as main_file:
    # Génération des 32 gros fichiers
    for i in range(1, NB_FICHIERS + 1):
        main_file.write(f'include "fichier_{i}.p";\n')
        
        with open(f"{DOSSIER}/fichier_{i}.p", "w") as f:
            for j in range(1, FONCTIONS_PAR_FICHIER + 1):
                f.write(f"fn int64 func_{i}_{j}() {{\n    return 1;\n}}\n")

    # Génération du main
    main_file.write("\nfn void executerTestMassif() {\n")
    main_file.write(f'   dec string[] msg = "Test 32 fichiers de {FONCTIONS_PAR_FICHIER * 4} lignes : ";\n')
    main_file.write('    call print(ref msg);\n')
    
   # on créer une function qui callle la dernière function de chaque fichier 
    calls = " + ".join([f"call func_{i}_{FONCTIONS_PAR_FICHIER}()" for i in range(1, NB_FICHIERS + 1)])
    main_file.write(f"    dec int64 resultat = {calls};\n")
    
    main_file.write("    call print(resultat);\n")
    main_file.write("    call backSlashN();\n")
    main_file.write("}\n")

    # on créer le main pour execuler le test massif

    main_file.write("fn int32 main()\n")
    main_file.write("{\n")
    main_file.write("    call executerTestMassif();\n")
    main_file.write("    return 0;\n")
    main_file.write("}\n")

print(f"Génération terminée dans '{DOSSIER}'. Résultat attendu : 32")