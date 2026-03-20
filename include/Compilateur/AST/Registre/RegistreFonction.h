#ifndef F2141F07_2C85_4ADB_9BC9_A909EBD34394
#define F2141F07_2C85_4ADB_9BC9_A909EBD34394

#include "RegistreGeneric.h"
#include <llvm-18/llvm/IR/Function.h>
#include <mutex>
#include <string>
#include <memory>

class IType;
class NoeudDeclarationFonction;

class ISymboleRegistreFonction
{
    public:
        enum class SymboleType { Globale, Locale };
        ISymboleRegistreFonction() = default;
        ISymboleRegistreFonction(const ISymboleRegistreFonction&) = delete;
        auto operator=(const ISymboleRegistreFonction&) -> ISymboleRegistreFonction& = delete;
        ISymboleRegistreFonction(ISymboleRegistreFonction&&) = delete;
        auto operator=(ISymboleRegistreFonction&&) -> ISymboleRegistreFonction& = delete;
        virtual ~ISymboleRegistreFonction() = default;
        [[nodiscard]] virtual SymboleType getType() const = 0;
};

// Fait office de struct pour stocker les fonctions globales donc 
// Je met en public 
// Pas de llvm::Function* ici, thread safe pour le registre global
class SymboleFonctionGlobale : public ISymboleRegistreFonction {
public: 
    IType* typeRetour = nullptr;
    NoeudDeclarationFonction* noeud = nullptr;
    
    [[nodiscard]] auto getType() const -> SymboleType override { return SymboleType::Globale; }
    [[nodiscard]] static auto classof(const ISymboleRegistreFonction* s) -> bool { 
        return s->getType() == SymboleType::Globale; 
    }
};

// Registre local avec llvm::Function* pour la génération de code dans un thread
class SymboleFonctionLocale : public ISymboleRegistreFonction {
public:
    llvm::Function* fonction = nullptr;
    IType* typeRetour = nullptr;
    NoeudDeclarationFonction* noeud = nullptr;

    [[nodiscard]] auto getType() const -> SymboleType override { return SymboleType::Locale; }
    [[nodiscard]] static auto classof(const ISymboleRegistreFonction* s) -> bool { 
        return s->getType() == SymboleType::Locale; 
    }
};

class RegistreFonctionGlobale : public RegistreGeneric<std::string, std::unique_ptr<ISymboleRegistreFonction>, std::mutex>
{
public:
    RegistreFonctionGlobale() = default;
    RegistreFonctionGlobale(const RegistreFonctionGlobale&) = delete;
    auto operator=(const RegistreFonctionGlobale&) -> RegistreFonctionGlobale& = delete;
    RegistreFonctionGlobale(RegistreFonctionGlobale&&) = delete;
    auto operator=(RegistreFonctionGlobale&&) -> RegistreFonctionGlobale& = delete;
    ~RegistreFonctionGlobale() override = default;
};

class RegistreFonctionLocale : public RegistreGeneric<std::string, std::unique_ptr<ISymboleRegistreFonction>>
{
public:
    RegistreFonctionLocale() = default;
    RegistreFonctionLocale(const RegistreFonctionLocale&) = delete;
    auto operator=(const RegistreFonctionLocale&) -> RegistreFonctionLocale& = delete;
    RegistreFonctionLocale(RegistreFonctionLocale&&) = delete;
    auto operator=(RegistreFonctionLocale&&) -> RegistreFonctionLocale& = delete;
    ~RegistreFonctionLocale() override = default;
};

#endif /* F2141F07_2C85_4ADB_9BC9_A909EBD34394 */
