import yaml
import os
from jinja2 import Environment, FileSystemLoader


class MoteurGeneration:

    def __init__(self, racine_projet):
        self._racine = racine_projet
        self._fichier_yaml = os.path.join(racine_projet, "include", "Compilateur", "AST", "YamelAST", "ast.yaml")
        self._env = Environment(
            loader=FileSystemLoader(os.path.join(racine_projet, "generation", "templates")),
            trim_blocks=True,
            lstrip_blocks=True
        )

    def _charger_noeuds_yaml(self):
        with open(self._fichier_yaml, "r", encoding="utf-8") as f:
            return yaml.safe_load(f).get("Noeud", {})

    def _rendre_et_ecrire(self, nom_template, chemin_sortie, **kwargs):
        contenu = self._env.get_template(nom_template).render(**kwargs)
        os.makedirs(os.path.dirname(chemin_sortie), exist_ok=True)
        with open(chemin_sortie, "w", encoding="utf-8") as f:
            f.write(contenu)

    @staticmethod
    def _extraire_traversables(champs):
        resultats = []
        for nom_champ, type_champ in champs.items():
            if type_champ in ("INoeud*", "std::vector<INoeud*>"):
                getter = "get" + nom_champ[0].upper() + nom_champ[1:]
                resultats.append((getter, type_champ))
        return resultats
