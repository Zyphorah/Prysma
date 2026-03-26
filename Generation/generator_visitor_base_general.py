import os
from Generation.engine_generation import EngineGeneration


class GenerateurVisitorBaseGenerale(EngineGeneration):

    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._fichier_entete = self._chemin_generation_include(
            "Compiler", "Visitor", "VisitorBaseGenerale.h"
        )
        self._fichier_source = self._chemin_generation_src(
            "Compiler", "Visitor", "VisitorBaseGenerale.cpp"
        )

    def generatedr(self):
        nodes = self._loadr_nodes_yaml()
        noms = list(nodes.keys()) + ["Instruction"]
        self._rendre_et_ecrire("visitor_base_general.h.j2", self._fichier_entete, nodes=noms)
        nodes_avec_champs = {}
        for nom, definition in nodes.items():
            champs = definition.get("champs", {}) if definition else {}
            if definition and definition.get("parent") == "NodeInstruction":
                champs_copie = dict(champs)
                champs_copie["children"] = "std::vector<INode*>"
                champs = champs_copie
            nodes_avec_champs[nom] = champs
        nodes_avec_champs["Instruction"] = {"children": "std::vector<INode*>"}
        methodes = {nom: self._extraire_traversables(champs) for nom, champs in nodes_avec_champs.items()}
        self._rendre_et_ecrire("visitor_base_general.cpp.j2", self._fichier_source, methodes=methodes)
