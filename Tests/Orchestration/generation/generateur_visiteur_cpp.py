import yaml
import os


class GenerateurVisiteurCpp:

    def __init__(self, racine_projet):
        self._racine = racine_projet
        self._fichier_yaml = os.path.join(racine_projet, "include", "Compilateur", "AST", "YamelAST", "ast.yaml")
        self._fichier_sortie = os.path.join(racine_projet, "src", "Compilateur", "Visiteur", "VisiteurBaseGenerale.cpp")

    def generer(self):
        noeuds = self._charger_noeuds()
        contenu = '#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"\n'
        contenu += '#include "Compilateur/AST/AST_Genere.h"\n\n'
        for nom, champs in noeuds.items():
            contenu += self._generer_methode(nom, champs)
        os.makedirs(os.path.dirname(self._fichier_sortie), exist_ok=True)
        with open(self._fichier_sortie, "w", encoding="utf-8") as f:
            f.write(contenu)

    def _charger_noeuds(self):
        with open(self._fichier_yaml, "r", encoding="utf-8") as f:
            yaml_data = yaml.safe_load(f).get("Noeud", {})
        noeuds = {}
        for nom, definition in yaml_data.items():
            champs = definition.get("champs", {}) if definition else {}
            noeuds[nom] = champs
        noeuds["Instruction"] = {"enfants": "std::vector<INoeud*>"}
        return noeuds

    def _generer_methode(self, nom, champs):
        param = "noeud" + nom
        classe = "Noeud" + nom
        champs_traversables = self._extraire_traversables(champs)

        if not champs_traversables:
            return (
                f"void VisiteurBaseGenerale::visiter({classe}* {param} [[maybe_unused]])\n"
                f"{{\n"
                f"}}\n\n"
            )

        contenu = f"void VisiteurBaseGenerale::visiter({classe}* {param})\n{{\n"
        for getter, type_champ in champs_traversables:
            if type_champ == "std::vector<INoeud*>":
                contenu += f"    for (auto* element : {param}->{getter}())\n"
                contenu += f"    {{\n"
                contenu += f"        element->accept(this);\n"
                contenu += f"    }}\n"
            else:
                contenu += f"    if ({param}->{getter}() != nullptr)\n"
                contenu += f"    {{\n"
                contenu += f"        {param}->{getter}()->accept(this);\n"
                contenu += f"    }}\n"
        contenu += "}\n\n"
        return contenu

    @staticmethod
    def _extraire_traversables(champs):
        resultats = []
        for nom_champ, type_champ in champs.items():
            if type_champ == "INoeud*":
                getter = "get" + nom_champ[0].upper() + nom_champ[1:]
                resultats.append((getter, type_champ))
            elif type_champ == "std::vector<INoeud*>":
                getter = "get" + nom_champ[0].upper() + nom_champ[1:]
                resultats.append((getter, type_champ))
        return resultats
