#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"

GestionnaireOperateur::GestionnaireOperateur(char symbole)
    : _suivant(nullptr), _gestionnaireParenthese(nullptr), _symbole(symbole) {}

void GestionnaireOperateur::definirGestionnaireParenthese(IGestionnaireParenthese* gestionnaireParenthese) {
    _gestionnaireParenthese = gestionnaireParenthese;
}

void GestionnaireOperateur::definirSuivant(IGestionnaireOperateur* suivant) {
    _suivant = suivant;
}

int GestionnaireOperateur::chercherOperateur(const std::string& equation) const {
    if (_gestionnaireParenthese == nullptr) return -1;
    return _gestionnaireParenthese->trouverDernierAuNiveauZero(equation, _symbole);
}

int GestionnaireOperateur::traiter(const std::string& equation) {
    int indice = chercherOperateur(equation);
    
    if (indice != -1) {
        return indice;
    }
    
    if (_suivant != nullptr) {
        return _suivant->traiter(equation);
    }
    
    return -1;
}
