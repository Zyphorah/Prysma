
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


class GeneratorVisitorBaseGeneral(EngineGeneration):

    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._fichier_entete = self._chemin_generation_include(
            "compiler", "visitor", "visitor_base_generale.h"
        )
        self._fichier_source = self._chemin_generation_src(
            "compiler", "visitor", "visitor_base_generale.cpp"
        )

    def generate(self):
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
