import os
from Generation.engine_generation import EngineGeneration

class GenerateurParser(EngineGeneration):
    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._dossier_squelettes = os.path.join(racine_project, "Src", "Compiler", "Parser")
 

    def generatedr(self):
        definitions = self._loadr_definitions_nodes()
        nodes = [
            nom for nom, donnees in definitions.items()
            if self._a_engine(donnees, "parser")
        ]
        for nom in nodes:
            chemin_output = self._resoudre_chemin_entete(nom, definitions.get(nom))
            self._rendre_et_ecrire("Parser.h.j2", chemin_output, nom=nom)
            self._generatedr_squelette_source_si_absent(nom, definitions.get(nom))

    def _resoudre_chemin_entete(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_parser(nom_node, donnees_node)
        return self._chemin_generation_include(
            self._chemin_depuis_categorie(categorie, f"Parser{nom_node}.h")
        )

    def _resoudre_chemin_squelette(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_parser(nom_node, donnees_node)
        if categorie == "Parser":
            return os.path.join(self._dossier_squelettes, f"Parser{nom_node}.cpp")
        return os.path.join(self._dossier_squelettes, categorie, f"Parser{nom_node}.cpp")

    def _generatedr_squelette_source_si_absent(self, nom_node, donnees_node=None):
        chemin_source = self._resoudre_chemin_squelette(nom_node, donnees_node)
        include_parser = self._resoudre_include_parser(nom_node, donnees_node)
        self._rendre_et_ecrire_si_absent(
            "Parser.cpp.j2",
            chemin_source,
            nom=nom_node,
            include_parser=include_parser,
        )

    def _resoudre_include_parser(self, nom_node, donnees_node=None):
        categorie = self._resoudre_categorie_parser(nom_node, donnees_node)
        return "/".join([
            "Compiler",
            categorie,
            f"Parser{nom_node}.h",
        ])

    def _resoudre_categorie_parser(self, nom_node, donnees_node=None):
        return self._resoudre_categorie_node(
            nom_node,
            donnees_node=donnees_node,
            cle_override="categorie_parser",
            categorie_defaut="Parser",
        )

    