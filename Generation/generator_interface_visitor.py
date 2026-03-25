import os
from Generation.engine_generation import EngineGeneration


class GenerateurInterfaceVisitor(EngineGeneration):

    def __init__(self, racine_project):
        super().__init__(racine_project)
        dossier = self._chemin_generation_include("Compiler", "Visitor")
        self._fichier_ivisitor = os.path.join(dossier, "Interfaces", "IVisitor.h")

    def generatedr(self):
        noms = list(self._loadr_nodes_yaml().keys()) + ["Instruction"]
        self._rendre_et_ecrire("ivisitor.h.j2", self._fichier_ivisitor, nodes=noms)
