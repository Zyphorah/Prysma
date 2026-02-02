#ifndef E962F97C_7F79_445E_9F65_097981CC00B4
#define E962F97C_7F79_445E_9F65_097981CC00B4

#include "Compilateur/Lexer/TokenType.h"
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <functional>

template <typename TValeur>
class RegistreGeneric {
protected:
    std::map<TokenType, TValeur> _elements;
    std::function<std::string(TokenType)> _messageErreurCallback;

public:
    RegistreGeneric() = default;
    virtual ~RegistreGeneric() = default;

   
    void setMessageErreur(std::function<std::string(TokenType)>&& callback) {
        _messageErreurCallback = std::move(callback);
    }

   
    void enregistrer(TokenType cle, TValeur valeur) {
        _elements[cle] = std::move(valeur);
    }

    TValeur recuperer(TokenType cle) {
        auto iterator = _elements.find(cle);
        if (iterator == _elements.end()) {
            std::string message = genererMessageErreur(cle);
            throw std::invalid_argument(message);
        }
        return iterator->second;
    }

    bool existe(TokenType cle) const {
        return _elements.count(cle) > 0;
    }

  
    std::set<TokenType> obtenirCles() const {
        std::set<TokenType> cles;
        for (const auto& pair : _elements) {
            cles.insert(pair.first);
        }
        return cles;
    }

protected:
 
    virtual std::string genererMessageErreur(TokenType cle) const {
        if (_messageErreurCallback) {
            return _messageErreurCallback(cle);
        }
        return std::string("Element inconnu: ") + std::to_string(cle);
    }
};


#endif /* E962F97C_7F79_445E_9F65_097981CC00B4 */
