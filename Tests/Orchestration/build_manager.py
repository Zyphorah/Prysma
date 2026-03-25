import subprocess


class BuildManager:

    @staticmethod
    def executer_commande(commande):

        if not isinstance(commande, list):
            raise ValueError("La commande doit être une liste de chaînes de caractères.")
        elif not all(isinstance(arg, str) for arg in commande):
            raise ValueError("Tous les éléments de la commande doivent être des chaînes de caractères.")
        elif not commande:
            raise ValueError("La commande ne peut pas être vide.")

        print(f"Exécution : {' '.join(commande)}")
        result = subprocess.run(commande, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        if result.returncode != 0:
            print(f"Error lors de l'exécution : {result.stderr}")
        return result