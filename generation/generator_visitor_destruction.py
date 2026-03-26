
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

class GeneratorVisitorDestruction(EngineGeneration):
    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._fichier_entete = self._chemin_generation_include(
            "compiler", "visitor", "visitor_destruction.h"
        )
    
    def generate(self):
        nodes = self._loadr_nodes_yaml()
        noms = list(nodes.keys()) + ["Instruction"]
        self._rendre_et_ecrire("visitor_destruction.h.j2", self._fichier_entete, nodes=noms)
