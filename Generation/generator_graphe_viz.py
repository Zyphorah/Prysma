import yaml
import os
from Generation.engine_generation import EngineGeneration


class GenerateurGraphViz(EngineGeneration):

    def __init__(self, racine_project):
        super().__init__(racine_project)
        self._fichier_graphviz_yaml = os.path.join(
            racine_project, "include", "Compiler", "AST", "YamelAST", "graphviz.yaml"
        )
        self._dossier_entete = self._chemin_generation_include(
            "Compiler", "Visitor", "ASTGraphViz"
        )
        self._dossier_source = self._chemin_generation_src(
            "Compiler", "Visitor", "ASTGraphViz"
        )

    def generatedr(self):
        nodes = self._loadr_nodes_yaml()
        config = self._loadr_config_graphviz()
        noms = list(nodes.keys()) + ["Instruction"]
        self._rendre_et_ecrire(
            "visitor_graphviz.h.j2",
            os.path.join(self._dossier_entete, "GeneralVisitorGraphViz.h"),
            nodes=noms
        )
        methodes = []
        for nom, definition in nodes.items():
            definition = definition or {}
            champs = definition.get("champs", {})
            label = self._deduire_label(nom, champs, config["labels_speciaux"])
            traversables = self._extraire_traversables(champs)
            for e in config["childs_herites"].get(nom, []):
                traversables.append((e["getter"], e["type"]))
            methodes.append((nom, label, traversables))
        self._rendre_et_ecrire(
            "visitor_graphviz.cpp.j2",
            os.path.join(self._dossier_source, "GeneralVisitorGraphViz.cpp"),
            methodes=methodes
        )

    def _loadr_config_graphviz(self):
        if not os.path.exists(self._fichier_graphviz_yaml):
            return {"labels_speciaux": {}, "childs_herites": {}}
        with open(self._fichier_graphviz_yaml, "r", encoding="utf-8") as f:
            donnees = yaml.safe_load(f) or {}
        return {
            "labels_speciaux": donnees.get("labels_speciaux", {}),
            "childs_herites": donnees.get("childs_herites", {})
        }

    @staticmethod
    def _deduire_label(nom_node, champs, labels_speciaux):
        if nom_node in labels_speciaux:
            return labels_speciaux[nom_node]

        for nom_champ, type_champ in champs.items():
            getter = "get" + nom_champ[0].upper() + nom_champ[1:]
            if type_champ == "std::string":
                return f'"{nom_node}: " + node->{getter}()'
            if type_champ == "Token":
                return f'node->{getter}().value'

        return f'"{nom_node}"'
