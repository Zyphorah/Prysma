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
private:
    std::map<TKey, TValeur> _elements;
    std::function<std::string(TKey)> _messageErreurCallback;
    mutable TLock _lock;
public:
    RegistreGeneric() = default;
    virtual ~RegistreGeneric() = default;

    RegistreGeneric(const RegistreGeneric&) = delete;
    auto operator=(const RegistreGeneric&) -> RegistreGeneric& = delete;
    RegistreGeneric(RegistreGeneric&&) = delete;
    auto operator=(RegistreGeneric&&) -> RegistreGeneric& = delete;
   
    void setMessageErreur(std::function<std::string(TKey)>&& callback) {
        std::lock_guard<TLock> guard(_lock);
        _messageErreurCallback = std::move(callback);
    }

   
    void enregistrer(const TKey& cle, TValeur valeur) {
        std::lock_guard<TLock> guard(_lock);
        _elements[cle] = std::move(valeur);
    }

    auto recuperer(const TKey& cle) const -> const TValeur& {
        std::lock_guard<TLock> guard(_lock);
        auto iterator = _elements.find(cle);
        if (iterator == _elements.end()) {
            throw std::invalid_argument(genererMessageErreurInterne(cle));
        }
        return iterator->second;
    }

    auto existe(const TKey& cle) const -> bool {
        std::lock_guard<TLock> guard(_lock);
        return _elements.count(cle) > 0;
    }

  
    auto obtenirCles() const -> std::set<TKey> {
        std::lock_guard<TLock> guard(_lock);
        std::set<TKey> cles;
        for (const auto& pair : _elements) {
            cles.insert(pair.first);
        }
        return cles;
    }

protected:
   virtual auto genererMessageErreur(const TKey& cle) const -> std::string {
        std::lock_guard<TLock> guard(_lock);
        return genererMessageErreurInterne(cle);
    }

private:
   auto genererMessageErreurInterne(const TKey& cle) const -> std::string {
        if (_messageErreurCallback) {
            return _messageErreurCallback(cle);
        }
        std::stringstream stringStream;
        stringStream << "Element inconnu dans le registre : " << cle;
        return stringStream.str();
    }
};

#endif
