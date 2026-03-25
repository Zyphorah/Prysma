import os
from Generation.engine_generation import EngineGeneration

class GenerateurVisitorDestruction(EngineGeneration):
    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._fichier_entete = self._chemin_generation_include(
            "Compiler", "Visitor", "VisitorDestruction.h"
        )
    
    def generatedr(self):
        nodes = self._loadr_nodes_yaml()
        noms = list(nodes.keys()) + ["Instruction"]
        self._rendre_et_ecrire("visitor_destruction.h.j2", self._fichier_entete, nodes=noms)
