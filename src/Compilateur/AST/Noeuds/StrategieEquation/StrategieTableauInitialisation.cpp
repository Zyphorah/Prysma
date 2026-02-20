#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include <vector>

StrategieTableauInitialisation::StrategieTableauInitialisation(IConstructeurArbre* constructeurArbreEquation)
    : _constructeurArbreEquation(constructeurArbreEquation)
{
}

// Calcule d'index pour les éléments du tableau, utilisée la virgule comme séparateur de sous équation à envoyer au constructeur d'arbre d'equation
INoeud* StrategieTableauInitialisation::construire(std::vector<Token>& equation)
{
    // equation commence avec le '[' - nous devons le sauter
    std::vector<INoeud*> elementsTableau;
    std::vector<Token> sousEquation;
    
    int index = 1;  // Commencer après le '['
    // Parcourir les éléments jusqu'au ']'
    while (index < static_cast<int>(equation.size()) && equation[index].type != TOKEN_CROCHET_FERME) {
        if (equation[index].type == TOKEN_VIRGULE) {
            index++;
            INoeud* element = _constructeurArbreEquation->construire(sousEquation);
            if (element != nullptr) {
                elementsTableau.push_back(element);
            }
            sousEquation.clear();
            continue;
        }

        sousEquation.push_back(equation[index]);
        index++;
    }
    
    // Retourner un conteneur avec tous les éléments
    if (!sousEquation.empty()) {
        INoeud* element = _constructeurArbreEquation->construire(sousEquation);
        if (element != nullptr) {
            elementsTableau.push_back(element);
        }
    }
    return _constructeurArbreEquation->allouer<NoeudTableauInitialisation>(elementsTableau);
}
