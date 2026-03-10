import os
from generation.moteur_generation import MoteurGeneration


class GenerateurExpression(MoteurGeneration):
    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        self._dossier_squelettes = os.path.join(
            racine_projet,
            "src",
            "Compilateur",
            "AST",
            "Noeuds",
            "Expression",
        )

    def generer(self):
        definitions = self._charger_definitions_noeuds()
        for nom, donnees in definitions.items():
            if not self._a_moteur(donnees, "expression"):
                continue
            chemin_entete = self._resoudre_chemin_entete(nom, donnees)
            chemin_source = self._resoudre_chemin_source(nom, donnees)
            self._rendre_et_ecrire(
                "expression.h.j2",
                chemin_entete,
                nom=nom,
            )
            self._rendre_et_ecrire_si_absent(
                "expression.cpp.j2",
                chemin_source,
                nom=nom,
                include_expression=self._resoudre_include_expression(nom, donnees),
            )

    def _resoudre_chemin_entete(self, nom_noeud, donnees_noeud=None):
        categorie = self._resoudre_categorie_expression(nom_noeud, donnees_noeud)
        return self._chemin_generation_include(
            "Compilateur",
            categorie,
            f"Expression{nom_noeud}.h",
        )

    def _resoudre_chemin_source(self, nom_noeud, donnees_noeud=None):
        categorie = self._resoudre_categorie_expression(nom_noeud, donnees_noeud)
        return os.path.join(self._dossier_squelettes, categorie, f"Expression{nom_noeud}.cpp")

    def _resoudre_include_expression(self, nom_noeud, donnees_noeud=None):
        categorie = self._resoudre_categorie_expression(nom_noeud, donnees_noeud)
        return "/".join([
            "Compilateur",
            categorie,
            f"Expression{nom_noeud}.h",
        ])

    def _resoudre_categorie_expression(self, nom_noeud, donnees_noeud=None):
        return self._resoudre_categorie_noeud(
            nom_noeud,
            donnees_noeud=donnees_noeud,
            cle_override="categorie_expression",
            categorie_defaut="Expression",
        )