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
        virtual ~ISymboleRegistreFonction() = default;
};

// Fait office de struct pour stocker les fonctions globales donc 
// Je met en public 
// Pas de llvm::Function* ici, thread safe pour le registre global
class SymboleFonctionGlobale : public ISymboleRegistreFonction {
public: 
    IType* typeRetour = nullptr;
    NoeudDeclarationFonction* noeud = nullptr;
};

// Registre local avec llvm::Function* pour la génération de code dans un thread
class SymboleFonctionLocale : public ISymboleRegistreFonction {
public:
    llvm::Function* fonction = nullptr;
    IType* typeRetour = nullptr;
    NoeudDeclarationFonction* noeud = nullptr;
};

class RegistreFonctionGlobale : public RegistreGeneric<std::string, std::unique_ptr<ISymboleRegistreFonction>, std::mutex>
{
public:
    RegistreFonctionGlobale() = default;
    ~RegistreFonctionGlobale() = default;
};

class RegistreFonctionLocale : public RegistreGeneric<std::string, std::unique_ptr<ISymboleRegistreFonction>>
{
public:
    RegistreFonctionLocale() = default;
    ~RegistreFonctionLocale() = default;
};

#endif /* F2141F07_2C85_4ADB_9BC9_A909EBD34394 */
