#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include <memory>
#include <vector>

std::shared_ptr<INoeud> StrategieTableauInitialisation::construire(std::vector<Token>& equation)
{
    // equation commence avec le '[' - nous devons le sauter
    std::vector<std::shared_ptr<INoeud>> elementsTableau;
    
    int index = 1;  // Commencer après le '['
    // Parcourir les éléments jusqu'au ']'
    while (index < static_cast<int>(equation.size()) && equation[index].type != TOKEN_CROCHET_FERME) {
        if (equation[index].type == TOKEN_VIRGULE) {
            index++;
            continue;
        }
        
        // Créer un littéral pour chaque élément
        if (equation[index].type == TOKEN_LIT_INT || 
            equation[index].type == TOKEN_LIT_FLOAT ||
            equation[index].type == TOKEN_LIT_BOLEEN) {
            std::shared_ptr<NoeudLitteral> litteral = std::make_shared<NoeudLitteral>(equation[index]);
            elementsTableau.push_back(litteral);
        }
        index++;
    }
    
    // Retourner un conteneur avec tous les éléments
    if (!elementsTableau.empty()) {
        return std::make_shared<NoeudTableauInitialisation>(elementsTableau);
    }
    
    return nullptr;
}
