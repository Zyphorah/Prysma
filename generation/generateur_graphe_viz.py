import yaml
import os
from generation.moteur_generation import MoteurGeneration


class GenerateurGrapheViz(MoteurGeneration):

    def __init__(self, racine_projet):
        super().__init__(racine_projet)
        self._fichier_graphviz_yaml = os.path.join(
            racine_projet, "include", "Compilateur", "AST", "YamelAST", "graphviz.yaml"
        )
        self._dossier_sortie = os.path.join(
            racine_projet, "build", "generationCode", "Compilateur", "Visiteur", "ASTGraphViz"
        )

    def generer(self):
        noeuds = self._charger_noeuds_yaml()
        config = self._charger_config_graphviz()
        noms = list(noeuds.keys()) + ["Instruction"]
        self._rendre_et_ecrire(
            "visiteur_graphviz.h.j2",
            os.path.join(self._dossier_sortie, "VisiteurGeneralGraphViz.h"),
            noeuds=noms
        )
        methodes = []
        for nom, definition in noeuds.items():
            definition = definition or {}
            champs = definition.get("champs", {})
            label = self._deduire_label(nom, champs, config["labels_speciaux"])
            traversables = self._extraire_traversables(champs)
            for e in config["enfants_herites"].get(nom, []):
                traversables.append((e["getter"], e["type"]))
            methodes.append((nom, label, traversables))
        self._rendre_et_ecrire(
            "visiteur_graphviz.cpp.j2",
            os.path.join(self._dossier_sortie, "VisiteurGeneralGraphViz.cpp"),
            methodes=methodes
        )

    def _charger_config_graphviz(self):
        if not os.path.exists(self._fichier_graphviz_yaml):
            return {"labels_speciaux": {}, "enfants_herites": {}}
        with open(self._fichier_graphviz_yaml, "r", encoding="utf-8") as f:
            donnees = yaml.safe_load(f) or {}
        return {
            "labels_speciaux": donnees.get("labels_speciaux", {}),
            "enfants_herites": donnees.get("enfants_herites", {})
        }

    @staticmethod
    def _deduire_label(nom_noeud, champs, labels_speciaux):
        if nom_noeud in labels_speciaux:
            return labels_speciaux[nom_noeud]

        for nom_champ, type_champ in champs.items():
            getter = "get" + nom_champ[0].upper() + nom_champ[1:]
            if type_champ == "std::string":
                return f'"{nom_noeud}: " + noeud->{getter}()'
            if type_champ == "Token":
                return f'noeud->{getter}().value'

        return f'"{nom_noeud}"'
