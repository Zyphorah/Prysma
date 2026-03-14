#ifndef E962F97C_7F79_445E_9F65_097981CC00B4
#define E962F97C_7F79_445E_9F65_097981CC00B4

#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <functional>
#include <sstream>
#include <mutex>

// Structure fantôme pour l'usage local (monothread)
struct RegistreLock {
    void lock() const{}
    void unlock() const {}
};

template <typename TKey, typename TValeur, typename TLock = RegistreLock>
class RegistreGeneric {
protected:
    std::map<TKey, TValeur> _elements;
    std::function<std::string(TKey)> _messageErreurCallback;
    mutable TLock _lock;
public:
    RegistreGeneric() = default;
    virtual ~RegistreGeneric() = default;
   
    void setMessageErreur(std::function<std::string(TKey)>&& callback) {
        std::lock_guard<TLock> guard(_lock);
        _messageErreurCallback = std::move(callback);
    }

   
    void enregistrer(const TKey& cle, TValeur valeur) {
        std::lock_guard<TLock> guard(_lock);
        _elements[cle] = std::move(valeur);
    }

    const TValeur& recuperer(const TKey& cle) const {
        std::lock_guard<TLock> guard(_lock);
        auto iterator = _elements.find(cle);
        if (iterator == _elements.end()) {
            throw std::invalid_argument(genererMessageErreurInterne(cle));
        }
        return iterator->second;
    }

    bool existe(const TKey& cle) const {
        std::lock_guard<TLock> guard(_lock);
        return _elements.count(cle) > 0;
    }

  
    std::set<TKey> obtenirCles() const {
        std::lock_guard<TLock> guard(_lock);
        std::set<TKey> cles;
        for (const auto& pair : _elements) {
            cles.insert(pair.first);
        }
        return cles;
    }

 

protected:
   virtual std::string genererMessageErreur(const TKey& cle) const {
        std::lock_guard<TLock> guard(_lock);
        return genererMessageErreurInterne(cle);
    }

private:
   std::string genererMessageErreurInterne(const TKey& cle) const {
        if (_messageErreurCallback) {
            return _messageErreurCallback(cle);
        }
        std::stringstream stringStream;
        stringStream << "Element inconnu dans le registre : " << cle;
        return stringStream.str();
    }
};


#endif
