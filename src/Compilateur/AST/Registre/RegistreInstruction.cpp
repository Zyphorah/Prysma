#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include <stdexcept>
#include <string>

void RegistreInstruction::enregistrer(
    TokenType typeToken,
    std::function<std::shared_ptr<INoeud>()> fournisseur) {
    _instructions[typeToken] = std::move(fournisseur);
}

std::shared_ptr<INoeud> RegistreInstruction::recuperer(TokenType typeToken) {
    auto iterator = _instructions.find(typeToken);
    if (iterator == _instructions.end()) {
        throw std::invalid_argument(std::string("Type de token inconnu: ") + std::to_string(typeToken));
    }
    return iterator->second();
}

bool RegistreInstruction::estInstruction(TokenType typeToken) const {
    return _instructions.count(typeToken) > 0;
}

std::set<TokenType> RegistreInstruction::obtenirTypesTokens() const {
    std::set<TokenType> typesTokens;
    for (const auto& pair : _instructions) {
        typesTokens.insert(pair.first);
    }
    return typesTokens;
}
