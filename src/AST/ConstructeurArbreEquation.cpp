#include "AST/ConstructeurArbreEquation.h"
#include "AST/Noeuds/Operande/Valeur.h"
#include "AST/Noeuds/Interfaces/IExpression.h"

ConstructeurArbreEquation::ConstructeurArbreEquation(
    ChaineResponsabilite* chaineResponsabilite,
    std::shared_ptr<RegistreSymbole> registreSymbole,
    IGestionnaireParenthese* gestionnaireParenthese)
    : _chaineResponsabilite(chaineResponsabilite), 
      _registreSymbole(registreSymbole), 
      _gestionnaireParenthese(gestionnaireParenthese) {}

std::shared_ptr<IExpression> ConstructeurArbreEquation::construire(std::string equation) {
    equation = _gestionnaireParenthese->enleverParenthesesEnglobantes(equation);
    
    int indice = _chaineResponsabilite->trouverOperateur(equation);
    
    if (indice == -1) {
        return std::make_shared<Valeur>(std::stod(equation));
    }
    
    auto noeud = _registreSymbole->recupererNoeud(equation[indice]);
    auto gauche = construire(equation.substr(0, indice));
    auto droite = construire(equation.substr(indice + 1));
    
    noeud->ajouterExpression(gauche, droite);
    return noeud;
}
