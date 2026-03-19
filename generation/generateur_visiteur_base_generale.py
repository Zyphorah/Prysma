import os
from generation.moteur_generation import MoteurGeneration


class GenerateurVisiteurBaseGenerale(MoteurGeneration):

    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        self._fichier_entete = self._chemin_generation_include(
            "Compilateur", "Visiteur", "VisiteurBaseGenerale.h"
        )
        self._fichier_source = self._chemin_generation_src(
            "Compilateur", "Visiteur", "VisiteurBaseGenerale.cpp"
        )

    def generer(self):
        noeuds = self._charger_noeuds_yaml()
        noms = list(noeuds.keys()) + ["Instruction"]
        self._rendre_et_ecrire("visiteur_base_generale.h.j2", self._fichier_entete, noeuds=noms)
        noeuds_avec_champs = {}
        for nom, definition in noeuds.items():
            champs = definition.get("champs", {}) if definition else {}
            if definition and definition.get("parent") == "NoeudInstruction":
                champs_copie = dict(champs)
                champs_copie["enfants"] = "std::vector<INoeud*>"
                champs = champs_copie
            noeuds_avec_champs[nom] = champs
        noeuds_avec_champs["Instruction"] = {"enfants": "std::vector<INoeud*>"}
        methodes = {nom: self._extraire_traversables(champs) for nom, champs in noeuds_avec_champs.items()}
        self._rendre_et_ecrire("visiteur_base_generale.cpp.j2", self._fichier_source, methodes=methodes)
