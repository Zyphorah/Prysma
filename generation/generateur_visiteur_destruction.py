import os
from generation.moteur_generation import MoteurGeneration

class GenerateurVisiteurDestruction(MoteurGeneration):
    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        self._fichier_entete = self._chemin_generation_include(
            "Compilateur", "Visiteur", "VisiteurDestruction.h"
        )
    
    def generer(self):
        noeuds = self._charger_noeuds_yaml()
        noms = list(noeuds.keys()) + ["Instruction"]
        self._rendre_et_ecrire("visiteur_destruction.h.j2", self._fichier_entete, noeuds=noms)
