#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"

ChaineResponsabilite::ChaineResponsabilite(
    IGestionnaireParenthese* gestionnaireParenthese, 
    std::vector<GestionnaireOperateur*> operateurs)
    : _debut(nullptr), _gestionnaireParenthese(gestionnaireParenthese), _operateurs(std::move(operateurs)) {
    
    // Chaîner les gestionnaires
    for (size_t i = 0; i < _operateurs.size(); i++) {
        _operateurs[i]->definirGestionnaireParenthese(_gestionnaireParenthese);
        if (i < _operateurs.size() - 1) {
            _operateurs[i]->definirSuivant(_operateurs[i + 1]);
        }
    }
    
    if (!_operateurs.empty()) {
        _debut = _operateurs[0];
    }
}

ChaineResponsabilite::~ChaineResponsabilite() {
    // Les gestionnaires ne sont pas supprimés ici, c'est au propriétaire de s'en charger
}

int ChaineResponsabilite::trouverOperateur(const std::string& equation) const {
    if (_debut == nullptr)
    {
        return -1;
    }
    return _debut->traiter(equation);
}
