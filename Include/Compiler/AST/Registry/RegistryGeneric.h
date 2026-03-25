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
struct RegistryLock {
    void lock() const{}
    void unlock() const {}
};

template <typename TKey, typename TValeur, typename TLock = RegistryLock>
class RegistryGeneric {
private:
    std::map<TKey, TValeur> _elements;
    std::function<std::string(TKey)> _messageErrorCallback;
    mutable TLock _lock;
public:
    RegistryGeneric() = default;
    virtual ~RegistryGeneric() = default;

    RegistryGeneric(const RegistryGeneric&) = delete;
    auto operator=(const RegistryGeneric&) -> RegistryGeneric& = delete;
    RegistryGeneric(RegistryGeneric&&) = delete;
    auto operator=(RegistryGeneric&&) -> RegistryGeneric& = delete;
   
    void setMessageError(std::function<std::string(TKey)>&& callback) {
        std::lock_guard<TLock> guard(_lock);
        _messageErrorCallback = std::move(callback);
    }

   
    void enregistryr(const TKey& cle, TValeur valeur) {
        std::lock_guard<TLock> guard(_lock);
        _elements[cle] = std::move(valeur);
    }

    auto recuperer(const TKey& cle) const -> const TValeur& {
        std::lock_guard<TLock> guard(_lock);
        auto iterator = _elements.find(cle);
        if (iterator == _elements.end()) {
            throw std::invalid_argument(generatedrMessageErrorInterne(cle));
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
   virtual auto generatedrMessageError(const TKey& cle) const -> std::string {
        std::lock_guard<TLock> guard(_lock);
        return generatedrMessageErrorInterne(cle);
    }

private:
   auto generatedrMessageErrorInterne(const TKey& cle) const -> std::string {
        if (_messageErrorCallback) {
            return _messageErrorCallback(cle);
        }
        std::stringstream stringStream;
        stringStream << "Element inconnu dans le registry : " << cle;
        return stringStream.str();
    }
};

#endif
