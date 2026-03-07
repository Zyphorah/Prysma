import yaml
import os

class GenerateurAST:

    def __init__(self, racine_projet):
        self._racine = racine_projet
        self._fichier_yaml = os.path.join(racine_projet, "include", "Compilateur", "AST", "YamelAST", "ast.yaml")
        self._fichier_sortie = os.path.join(racine_projet, "include", "Compilateur", "AST", "AST_Genere.h")

    def generer(self):
        noeuds = self._charger_yaml()
        contenu = self._generer_entete()
        contenu += self._generer_forward_declarations(noeuds)
        for nom, data in noeuds.items():
            contenu += self._generer_classe(nom, data)
        contenu += "#endif\n"
        self._ecrire_fichier(contenu)

    def _charger_yaml(self):
        with open(self._fichier_yaml, "r", encoding="utf-8") as f:
            return yaml.safe_load(f).get("Noeud", {})

    def _generer_entete(self):
        return (
            "#ifndef AST_GENERE_H\n"
            "#define AST_GENERE_H\n\n"
            '#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"\n'
            '#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"\n'
            '#include "Compilateur/AST/Noeuds/NoeudInstruction.h"\n'
            '#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"\n'
            '#include "Compilateur/Lexer/Lexer.h"\n'
            '#include <string>\n'
            '#include <vector>\n'
            '#include <utility>\n'
            '#include "Compilateur/AST/Registre/Types/IType.h"\n\n'
        )

    def _generer_forward_declarations(self, noeuds):
        lignes = [f"class Noeud{nom};\n" for nom in noeuds]
        lignes.append("\n")
        return "".join(lignes)

    def _generer_classe(self, nom, data):
        parent, champs = self._extraire_donnees(data)
        nom_classe = f"Noeud{nom}"
        contenu = f"class {nom_classe} : public {parent} {{\n"
        contenu += self._generer_attributs(champs)
        contenu += self._generer_constructeurs(nom_classe, champs)
        contenu += self._generer_methodes(nom_classe, parent, champs)
        contenu += self._generer_getters(champs)
        contenu += "};\n\n"
        return contenu

    def _extraire_donnees(self, data):
        if data is None:
            data = {}
        parent = data.get("parent", "INoeud")
        champs = data.get("champs", {})
        if not champs and "parent" not in data:
            champs = data
        return parent, champs

    def _generer_attributs(self, champs):
        contenu = "private:\n"
        for nom, type_c in champs.items():
            contenu += f"    {type_c} _{nom};\n"
        contenu += "\n"
        return contenu

    def _generer_constructeurs(self, nom_classe, champs):
        contenu = "public:\n"
        contenu += f"    {nom_classe}() = default;\n"
        if not champs:
            return contenu

        items = list(champs.items())
        debut_defaut = self._trouver_debut_trailing_defaults(items)
        params, inits = [], []

        for idx, (nom, type_c) in enumerate(items):
            type_param, fmt_init = self._analyser_parametre(type_c)
            defaut = self._valeur_defaut(type_c)
            if defaut is not None and idx >= debut_defaut:
                params.append(f"{type_param} {nom} = {defaut}")
            else:
                params.append(f"{type_param} {nom}")
            inits.append(f"_{nom}({fmt_init.format(nom=nom)})")

        contenu += f"    {nom_classe}({', '.join(params)}) : {', '.join(inits)} {{}}\n"
        return contenu

    def _generer_methodes(self, nom_classe, parent, champs):
        contenu = f"    ~{nom_classe}() override = default;\n\n"
        contenu += "    void accept(IVisiteur* visiteur) override {\n"
        contenu += "        visiteur->visiter(this);\n"
        contenu += "    }\n\n"
        if parent == "IExpression":
            contenu += "    IExpression* ajouterExpression(INoeud* gauche, INoeud* droite) override {\n"
            if "gauche" in champs:
                contenu += "        _gauche = gauche;\n"
            if "droite" in champs:
                contenu += "        _droite = droite;\n"
            contenu += "        return this;\n"
            contenu += "    }\n\n"
        return contenu

    def _generer_getters(self, champs):
        contenu = ""
        for nom, type_c in champs.items():
            ret_type = self._type_retour_getter(type_c)
            nom_maj = nom[0].upper() + nom[1:]
            contenu += f"    {ret_type} get{nom_maj}() const {{ return _{nom}; }}\n"
        return contenu

    def _trouver_debut_trailing_defaults(self, items):
        debut = len(items)
        for i in range(len(items) - 1, -1, -1):
            if self._valeur_defaut(items[i][1]) is not None:
                debut = i
            else:
                break
        return debut

    @staticmethod
    def _analyser_parametre(type_c):
        if "std::" in type_c or type_c == "Token":
            return type_c, "std::move({nom})"
        return type_c, "{nom}"

    @staticmethod
    def _valeur_defaut(type_c):
        if "std::vector" in type_c:
            return "{}"
        if "*" in type_c:
            return "nullptr"
        return None

    @staticmethod
    def _type_retour_getter(type_c):
        if "*" in type_c or type_c in ["int", "float", "double", "bool", "char", "TokenType"]:
            return type_c
        return f"const {type_c}&"

    def _ecrire_fichier(self, contenu):
        with open(self._fichier_sortie, "w", encoding="utf-8") as f:
            f.write(contenu)
