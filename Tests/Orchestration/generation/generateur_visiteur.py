import yaml
import os

class GenerateurVisiteur:

    NOEUDS_EXTRA = ["Instruction"]

    def __init__(self, racine_projet):
        self._racine = racine_projet
        self._fichier_yaml = os.path.join(racine_projet, "include", "Compilateur", "AST", "YamelAST", "ast.yaml")
        self._fichier_ivisiteur = os.path.join(racine_projet, "include", "Compilateur", "Visiteur", "Interfaces", "IVisiteur.h")
        self._fichier_base = os.path.join(racine_projet, "include", "Compilateur", "Visiteur", "VisiteurBaseGenerale.h")

    def generer(self):
        noeuds = self._charger_noeuds()
        self._generer_ivisiteur(noeuds)
        self._generer_visiteur_base(noeuds)

    def _charger_noeuds(self):
        with open(self._fichier_yaml, "r", encoding="utf-8") as f:
            noeuds_yaml = list(yaml.safe_load(f).get("Noeud", {}).keys())
        return noeuds_yaml + self.NOEUDS_EXTRA

    def _generer_ivisiteur(self, noeuds):
        contenu = "#ifndef IVISITEUR_H\n"
        contenu += "#define IVISITEUR_H\n\n"

        for nom in noeuds:
            contenu += f"class Noeud{nom};\n"
        contenu += "\n"

        contenu += "class IVisiteur\n{\npublic:\n"
        contenu += "    virtual ~IVisiteur() = default;\n"

        for nom in noeuds:
            param = self._nom_parametre(nom)
            contenu += f"    virtual void visiter(Noeud{nom}* {param}) = 0;\n"

        contenu += "};\n\n"
        contenu += "#endif\n"
        self._ecrire_fichier(self._fichier_ivisiteur, contenu)

    def _generer_visiteur_base(self, noeuds):
        contenu = "#ifndef VISITEUR_BASE_GENERALE_H\n"
        contenu += "#define VISITEUR_BASE_GENERALE_H\n\n"
        contenu += '#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"\n'
        contenu += '#include "Compilateur/AST/AST_Genere.h"\n\n'

        for nom in noeuds:
            contenu += f"class Noeud{nom};\n"
        contenu += "\n"

        contenu += "class VisiteurBaseGenerale : public IVisiteur\n{\npublic:\n"

        for nom in noeuds:
            param = self._nom_parametre(nom)
            contenu += f"    void visiter(Noeud{nom}* {param}) override;\n"

        contenu += "};\n\n"
        contenu += "#endif\n"
        self._ecrire_fichier(self._fichier_base, contenu)

    @staticmethod
    def _nom_parametre(nom_noeud):
        return "noeud" + nom_noeud

    @staticmethod
    def _ecrire_fichier(chemin, contenu):
        with open(chemin, "w", encoding="utf-8") as f:
            f.write(contenu)
