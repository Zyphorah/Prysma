#include "Compilateur/AST/Noeuds/Operande/RegistreSymbole.h"
#include <stdexcept>
#include <string>

void RegistreSymbole::enregistrer(
    TokenType symbole, 
    std::function<std::shared_ptr<INoeud>()> fournisseur) {
    _carteSymboles[symbole] = fournisseur;
}

std::shared_ptr<INoeud> RegistreSymbole::recupererNoeud(TokenType symbole) {
    auto iterator = _carteSymboles.find(symbole);
    if (iterator == _carteSymboles.end()) {
        throw std::invalid_argument(std::string("Symbole inconnu: ") + std::to_string(symbole));
    }
    return iterator->second();
}

bool RegistreSymbole::estOperateur(TokenType symbole) const {
    return _carteSymboles.count(symbole) > 0;
}

std::set<TokenType> RegistreSymbole::obtenirSymboles() const {
    std::set<TokenType> symboles;
    for (const auto& pair : _carteSymboles) {
        symboles.insert(pair.first);
    }
    return symboles;
}
