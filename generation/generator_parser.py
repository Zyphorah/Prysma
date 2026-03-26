
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

class GeneratorParser(EngineGeneration):
    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._dossier_squelettes = os.path.join(racine_project, "src", "compiler", "parser")
 

    def generate(self):
        definitions = self._loadr_definitions_nodes()
        nodes = [
            nom for nom, donnees in definitions.items()
            if self._a_engine(donnees, "parser")
        ]
        for nom in nodes:
            chemin_output = self._resoudre_chemin_entete(nom, definitions.get(nom))
            self._rendre_et_ecrire("parser.h.j2", chemin_output, nom=nom)
            self._generatedr_squelette_source_si_absent(nom, definitions.get(nom))

    def _resoudre_chemin_entete(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_parser(nom_node, donnees_node); categorie = to_snake_case(categorie)
        return self._chemin_generation_include(
            self._chemin_depuis_categorie(categorie, to_snake_case(f"Parser{nom_node}.h"))
        )

    def _resoudre_chemin_squelette(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_parser(nom_node, donnees_node); categorie = to_snake_case(categorie)
        if categorie == "parser":
            return os.path.join(self._dossier_squelettes, to_snake_case(f"Parser{nom_node}.cpp"))
        return os.path.join(self._dossier_squelettes, categorie, to_snake_case(f"Parser{nom_node}.cpp"))

    def _generatedr_squelette_source_si_absent(self, nom_node, donnees_node=None):
        chemin_source = self._resoudre_chemin_squelette(nom_node, donnees_node)
        include_parser = self._resoudre_include_parser(nom_node, donnees_node)
        self._rendre_et_ecrire_si_absent(
            "parser.cpp.j2",
            chemin_source,
            nom=nom_node,
            include_parser=include_parser,
        )

    def _resoudre_include_parser(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_parser(nom_node, donnees_node); categorie = to_snake_case(categorie)
        return "/".join([
            "compiler",
            categorie,
            to_snake_case(f"Parser{nom_node}.h"),
        ])

    def _resoudre_categorie_parser(self, nom_node, donnees_node=None):
        return self._resoudre_categorie_node(
            nom_node,
            donnees_node=donnees_node,
            cle_override="categorie_parser",
            categorie_defaut="parser",
        )

    