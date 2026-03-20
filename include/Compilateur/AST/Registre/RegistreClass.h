#ifndef C2ADEE91_A0DA_4404_8AF5_5B1105A499EC
#define C2ADEE91_A0DA_4404_8AF5_5B1105A499EC

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <string>
#include <map>

class INoeud;

struct Class
{
private:
    RegistreFonctionLocale* registreFonctionLocale;
    RegistreVariable* registreVariable;

    llvm::GlobalVariable* vtable;
    
    // Pour calculer la taille automatique de l'objet avec llvm 
    llvm::StructType* structType;
    INoeud* parentHeritage;
    
    // Mapping des indices des membres pour la Passe 3
    std::map<std::string, unsigned int> memberIndices;
    std::map<std::string, INoeud*> memberInitializers;
    
    // Mapping des indices des méthodes dans la vtable
    std::map<std::string, unsigned int> methodIndices;

public:
    // Constructeur pour initialiser les membres
    Class(
        RegistreFonctionLocale* p_registreFonctionLocale,
        RegistreVariable* p_registreVariable,
        llvm::GlobalVariable* p_vtable,
        llvm::StructType* p_structType,
        INoeud* p_parentHeritage
    )
        : registreFonctionLocale(p_registreFonctionLocale),
          registreVariable(p_registreVariable),
          vtable(p_vtable),
          structType(p_structType),
          parentHeritage(p_parentHeritage)
    {}

    Class(const Class&) = delete;
    auto operator=(const Class&) -> Class& = delete;
    Class(Class&&) = delete;
    auto operator=(Class&&) -> Class& = delete;

    ~Class() {
        delete registreFonctionLocale;
        delete registreVariable;
    }

    Class() : registreFonctionLocale(nullptr), registreVariable(nullptr), vtable(nullptr), structType(nullptr), parentHeritage(nullptr) {}

    void setStructType(llvm::StructType* type) { structType = type; }
    void setParentHeritage(INoeud* noeud) { parentHeritage = noeud; }
    void setRegistreVariable(RegistreVariable* reg) { registreVariable = reg; }
    void setRegistreFonctionLocale(RegistreFonctionLocale* reg) { registreFonctionLocale = reg; }
    void setVTable(llvm::GlobalVariable* vtablePtr) { vtable = vtablePtr; }

    // Getters
    [[nodiscard]] auto getRegistreFonctionLocale() const -> RegistreFonctionLocale* { return registreFonctionLocale; }
    [[nodiscard]] auto getRegistreVariable() const -> RegistreVariable* { return registreVariable; }
    [[nodiscard]] auto getVTable() const -> llvm::GlobalVariable* { return vtable; }
    [[nodiscard]] auto getStructType() const -> llvm::StructType* { return structType; }
    [[nodiscard]] auto getParentHeritage() const -> INoeud* { return parentHeritage; }
    [[nodiscard]] auto getMemberIndices() -> std::map<std::string, unsigned int>& { return memberIndices; }
    [[nodiscard]] auto getMemberInitializers() -> std::map<std::string, INoeud*>& { return memberInitializers; }
    [[nodiscard]] auto getMethodIndices() -> std::map<std::string, unsigned int>& { return methodIndices; }
};
// TODO : supporter les mutex multi thread pour les classes Sinon le multi fichier n'est pas possible

#include <memory>

class RegistreClass : public RegistreGeneric<std::string, std::unique_ptr<Class>>
{
public:
    RegistreClass() = default;
    RegistreClass(const RegistreClass&) = delete;
    auto operator=(const RegistreClass&) -> RegistreClass& = delete;
    RegistreClass(RegistreClass&&) = delete;
    auto operator=(RegistreClass&&) -> RegistreClass& = delete;

    ~RegistreClass() override = default;
};


#endif /* C2ADEE91_A0DA_4404_8AF5_5B1105A499EC */
