import os
from Generation.engine_generation import EngineGeneration


class GenerateurExpression(EngineGeneration):
    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._dossier_squelettes = os.path.join(
            racine_project,
            "Src",
            "Compiler",
            "AST",
            "Nodes",
            "Expression",
        )

    def generatedr(self):
        definitions = self._loadr_definitions_nodes()
        for nom, donnees in definitions.items():
            if not self._a_engine(donnees, "expression"):
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

    def _resoudre_chemin_entete(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_expression(nom_node, donnees_node)
        return self._chemin_generation_include(
            "Compiler",
            categorie,
            f"Expression{nom_node}.h",
        )

    def _resoudre_chemin_source(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_expression(nom_node, donnees_node)
        return os.path.join(self._dossier_squelettes, categorie, f"Expression{nom_node}.cpp")

    def _resoudre_include_expression(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_expression(nom_node, donnees_node)
        return "/".join([
            "Compiler",
            categorie,
            f"Expression{nom_node}.h",
        ])

    def _resoudre_categorie_expression(self, nom_node, donnees_node=None):
        return self._resoudre_categorie_node(
            nom_node,
            donnees_node=donnees_node,
            cle_override="categorie_expression",
            categorie_defaut="Expression",
        )