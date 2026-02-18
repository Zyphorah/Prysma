#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include <memory>
#include <vector>

StrategieTableauInitialisation::StrategieTableauInitialisation(IConstructeurArbre* constructeurArbreEquation)
    : _constructeurArbreEquation(constructeurArbreEquation)
{
}

// Calcule d'index pour les éléments du tableau, utilisée la virgule comme séparateur de sous équation à envoyer au constructeur d'arbre d'equation
std::shared_ptr<INoeud> StrategieTableauInitialisation::construire(std::vector<Token>& equation)
{
    // equation commence avec le '[' - nous devons le sauter
    std::vector<std::shared_ptr<INoeud>> elementsTableau;
    std::vector<Token> sousEquation;
    
    int index = 1;  // Commencer après le '['
    // Parcourir les éléments jusqu'au ']'
    while (index < static_cast<int>(equation.size()) && equation[index].type != TOKEN_CROCHET_FERME) {
        if (equation[index].type == TOKEN_VIRGULE) {
            index++;
            std::shared_ptr<INoeud> element = _constructeurArbreEquation->construire(sousEquation);
            if (element) {
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
        std::shared_ptr<INoeud> element = _constructeurArbreEquation->construire(sousEquation);
        if (element) {
            elementsTableau.push_back(element);
        }
    }
    return std::make_shared<NoeudTableauInitialisation>(elementsTableau);
}
