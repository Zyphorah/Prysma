import os
from generation.moteur_generation import MoteurGeneration

class GenerateurParseur(MoteurGeneration):
    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        self.dossier = os.path.join(racine_projet, "build", "generationCode","Compilateur", "AnalyseSyntaxique")
 

    def generer(self):
        noeuds = list(self._charger_noeuds_yaml().keys()) + ["Instruction"] + ["Equation"]
        for nom in noeuds:
            chemin_sortie = os.path.join(self.dossier, f"Parseur{nom}.h")
            self._rendre_et_ecrire("Parseur.h.j2", chemin_sortie, nom=nom)

    