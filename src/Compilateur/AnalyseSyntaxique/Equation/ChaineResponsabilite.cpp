#include "Compilateur/AnalyseSyntaxique/Equation/ChaineResponsabilite.h"
#include "Compilateur/AnalyseSyntaxique/Equation/GestionnaireOperateur.h"
#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireParenthese.h"
#include "Compilateur/Lexer/Lexer.h"
#include <cstddef>
#include <utility>
#include <vector>

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

ChaineResponsabilite::~ChaineResponsabilite() = default;

auto ChaineResponsabilite::trouverOperateur(const std::vector<Token>& equation) const -> int {
    if (_debut == nullptr) {
        return -1;
    }
    return _debut->traiter(equation);
}
