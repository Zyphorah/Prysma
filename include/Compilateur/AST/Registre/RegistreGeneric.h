#ifndef E962F97C_7F79_445E_9F65_097981CC00B4
#define E962F97C_7F79_445E_9F65_097981CC00B4

#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <functional>
#include <sstream>

template <typename TKey, typename TValeur>
class RegistreGeneric {
protected:
    std::map<TKey, TValeur> _elements;
    std::function<std::string(TKey)> _messageErreurCallback;

public:
    RegistreGeneric() = default;
    virtual ~RegistreGeneric() = default;

   
    void setMessageErreur(std::function<std::string(TKey)>&& callback) {
        _messageErreurCallback = std::move(callback);
    }

   
    void enregistrer(TKey cle, TValeur valeur) {
        _elements[cle] = std::move(valeur);
    }

    TValeur recuperer(TKey cle) {
        auto iterator = _elements.find(cle);
        if (iterator == _elements.end()) {
            std::string message = genererMessageErreur(cle);
            throw std::invalid_argument(message);
        }
        return iterator->second;
    }

    bool existe(TKey cle) const {
        return _elements.count(cle) > 0;
    }

  
    std::set<TKey> obtenirCles() const {
        std::set<TKey> cles;
        for (const auto& pair : _elements) {
            cles.insert(pair.first);
        }
        return cles;
    }

protected:
 
   virtual std::string genererMessageErreur(const TKey& cle) const {
        std::stringstream stringStream;
        stringStream << "Element inconnu dans le registre : " << cle;
        return stringStream.str();
    }
};


#endif /* E962F97C_7F79_445E_9F65_097981CC00B4 */
