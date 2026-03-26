
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


class GeneratorInterfaceVisitor(EngineGeneration):

    def __init__(self, racine_project):
        super().__init__(racine_project)
        dossier = self._chemin_generation_include("compiler", "visitor")
        self._fichier_ivisitor = os.path.join(dossier, "interfaces", "i_visitor.h")

    def generate(self):
        noms = list(self._loadr_nodes_yaml().keys()) + ["Instruction"]
        self._rendre_et_ecrire("ivisitor.h.j2", self._fichier_ivisitor, nodes=noms)
