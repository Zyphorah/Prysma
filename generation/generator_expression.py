
import re
def to_snake_case(name):
    if '.' in name:
        base, ext = name.rsplit('.', 1)
        s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', base)
        s2 = re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()
        return f"{re.sub(r'[\s\-]+', '_', s2).strip('_')}.{ext.lower()}"
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    s2 = re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()
    return re.sub(r'[\s\-]+', '_', s2).strip('_')
import os
from generation.engine_generation import EngineGeneration


class GeneratorExpression(EngineGeneration):
    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._dossier_squelettes = os.path.join(
            racine_project,
            "src",
            "compiler",
            "ast",
            "nodes",
            "expression",
        )

    def generate(self):
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
        categorie = self._resoudre_categorie_expression(nom_node, donnees_node); categorie = to_snake_case(categorie)
        return self._chemin_generation_include(
            "compiler",
            categorie,
            to_snake_case(f"Expression{nom_node}.h"),
        )

    def _resoudre_chemin_source(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_expression(nom_node, donnees_node); categorie = to_snake_case(categorie)
        return os.path.join(self._dossier_squelettes, categorie, to_snake_case(f"Expression{nom_node}.cpp"))

    def _resoudre_include_expression(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_expression(nom_node, donnees_node); categorie = to_snake_case(categorie)
        return "/".join([
            "compiler",
            categorie,
            to_snake_case(f"Expression{nom_node}.h"),
        ])

    def _resoudre_categorie_expression(self, nom_node, donnees_node=None):
        return self._resoudre_categorie_node(
            nom_node,
            donnees_node=donnees_node,
            cle_override="categorie_expression",
            categorie_defaut="expression",
        )