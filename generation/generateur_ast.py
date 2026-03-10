import os
from generation.moteur_generation import MoteurGeneration


_CLES_METADONNEES_NOEUD = {
    "parent",
    "champs",
    "moteur",
}


class GenerateurAST(MoteurGeneration):

    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        self._fichier_sortie = self._chemin_generation_include(
            "Compilateur", "AST", "AST_Genere.h"
        )

    def generer(self):
        noeuds_data = self._charger_noeuds_yaml()
        classes = self._preparer_classes(noeuds_data)
        self._rendre_et_ecrire(
            "ast_genere.h.j2", self._fichier_sortie,
            noeuds_noms=list(noeuds_data.keys()), classes=classes
        )

    def _preparer_classes(self, noeuds):
        classes = []
        for nom, donnees in noeuds.items():
            parent, champs = self._extraire_donnees(donnees)
            liste_champs = list(champs.items())
            debut_defaut = self._trouver_debut_trailing_defaults(liste_champs)
            liste_params, liste_inits = [], []

            for idx, (nom_champ, type_c) in enumerate(liste_champs):
                type_param, fmt_init = self._analyser_parametre(type_c)
                defaut = self._valeur_defaut(type_c)
                if defaut is not None and idx >= debut_defaut:
                    liste_params.append(f"{type_param} {nom_champ} = {defaut}")
                else:
                    liste_params.append(f"{type_param} {nom_champ}")
                liste_inits.append(f"_{nom_champ}({fmt_init.format(nom=nom_champ)})")

            getters = {n: self._type_retour_getter(t) for n, t in champs.items()}

            classes.append({
                "nom": nom,
                "parent": parent,
                "champs": champs,
                "params_constructeur": ", ".join(liste_params),
                "inits_constructeur": ", ".join(liste_inits),
                "getters": getters
            })
        return classes

    @staticmethod
    def _extraire_donnees(donnees):
        if donnees is None:
            donnees = {}
        parent = donnees.get("parent", "INoeud")
        champs = donnees.get("champs", {})
        if champs is None:
            champs = {}
        if not champs and "parent" not in donnees:
            champs = {
                cle: valeur
                for cle, valeur in donnees.items()
                if cle not in _CLES_METADONNEES_NOEUD
            }
        return parent, champs

    def _trouver_debut_trailing_defaults(self, liste_champs):
        debut = len(liste_champs)
        for i in range(len(liste_champs) - 1, -1, -1):
            if self._valeur_defaut(liste_champs[i][1]) is not None:
                debut = i
            else:
                break
        return debut

    @staticmethod
    def _analyser_parametre(type_c):
        if "std::" in type_c or type_c == "Token":
            return type_c, "std::move({nom})"
        return type_c, "{nom}"

    @staticmethod
    def _valeur_defaut(type_c):
        if "std::vector" in type_c:
            return "{}"
        if "*" in type_c:
            return "nullptr"
        return None

    @staticmethod
    def _type_retour_getter(type_c):
        if "*" in type_c or type_c in ["int", "float", "double", "bool", "char", "TokenType"]:
            return type_c
        return f"const {type_c}&"
