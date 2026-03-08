import os
from generation.moteur_generation import MoteurGeneration


class GenerateurInterfaceVisiteur(MoteurGeneration):

    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        dossier = os.path.join(racine_projet, "build", "generationCode", "Compilateur", "Visiteur")
        self._fichier_ivisiteur = os.path.join(dossier, "Interfaces", "IVisiteur.h")

    def generer(self):
        noms = list(self._charger_noeuds_yaml().keys()) + ["Instruction"]
        self._rendre_et_ecrire("ivisiteur.h.j2", self._fichier_ivisiteur, noeuds=noms)
