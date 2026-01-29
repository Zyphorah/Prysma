#pragma once

#include "Compilateur/AST/Registre/Interfaces/IRegistreSymbole.h"
#include <functional>
#include <map>
#include <memory>
#include <set>

class RegistreSymbole : public IRegistreSymbole {
private:
    // LinkedMap conserve l'ordre d'insertion
    std::map<TokenType, std::function<std::shared_ptr<IExpression>()>> _carteSymboles;
    
public:

    void enregistrer(
        TokenType symbole, 
        std::function<std::shared_ptr<IExpression>()> fournisseur
    );

    std::shared_ptr<IExpression> recupererNoeud(TokenType symbole);
    bool estOperateur(TokenType symbole) const;
    std::set<TokenType> obtenirSymboles() const;
};

