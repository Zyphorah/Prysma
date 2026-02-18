#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudLectureTableau.h" 
#include "Compilateur/Lexer/TokenType.h"
#include <vector>

StrategieIdentifiant::StrategieIdentifiant(std::shared_ptr<IConstructeurArbre> constructeurArbre)
    : _constructeurArbre(std::move(constructeurArbre)) {}

StrategieIdentifiant::~StrategieIdentifiant()
{}

std::shared_ptr<INoeud> StrategieIdentifiant::construire(std::vector<Token>& equation) {
    bool isTableau = false;
    int indexCrochet = 0;
    std::vector<Token> EquationIndex;

    // Utilisation d'une technique de contexte, regarder en avant pour voir si il y a un []
    // Parcourir l'équation pour trouver le token crochet ouvrant
    for (int i = 0; i < equation.size(); i++) {
        if (equation[i].type == TOKEN_CROCHET_OUVERT) {
            indexCrochet = i;
            isTableau = true;
            break;
        }
    }

    if (isTableau) {
        // remplir le tableau de l'équation de l'index du crochet ouvrant jusqu'au crochet fermant correspondant
        for (int i = indexCrochet; i < equation.size(); i++) {
            if (equation[i].type == TOKEN_CROCHET_FERME) {
                break;
            }
            if (i != indexCrochet) { // ne pas ajouter le crochet ouvrant
                EquationIndex.push_back(equation[i]);
            }
        }

        std::shared_ptr<INoeud> indexEquation = _constructeurArbre->construire(EquationIndex);
        return std::make_shared<NoeudLectureTableau>(indexEquation, equation[0].value);
    }
  
    return std::make_shared<NoeudUnRefVariable>(equation[0].value);
}