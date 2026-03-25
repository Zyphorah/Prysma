#ifndef C2ADEE91_A0DA_4404_8AF5_5B1105A499EC
#define C2ADEE91_A0DA_4404_8AF5_5B1105A499EC

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Registry/RegistryGeneric.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <string>
#include <map>

class INode;

struct Class
{
private:
    RegistryFunctionLocale* registryFunctionLocale;
    RegistryVariable* registryVariable;

    llvm::GlobalVariable* vtable;
    
    // Pour calculer la taille automatique de l'object avec llvm 
    llvm::StructType* structType;
    INode* parentHeritage;
    
    // Mapping des indices des membres pour la Passe 3
    std::map<std::string, unsigned int> memberIndices;
    std::map<std::string, INode*> memberInitializers;
    
    // Mapping des indices des méthodes dans la vtable
    std::map<std::string, unsigned int> methodIndices;

public:
    // Builder pour initialiser les membres
    Class(
        RegistryFunctionLocale* p_registryFunctionLocale,
        RegistryVariable* p_registryVariable,
        llvm::GlobalVariable* p_vtable,
        llvm::StructType* p_structType,
        INode* p_parentHeritage
    )
        : registryFunctionLocale(p_registryFunctionLocale),
          registryVariable(p_registryVariable),
          vtable(p_vtable),
          structType(p_structType),
          parentHeritage(p_parentHeritage)
    {}

    Class(const Class&) = delete;
    auto operator=(const Class&) -> Class& = delete;
    Class(Class&&) = delete;
    auto operator=(Class&&) -> Class& = delete;

    ~Class() {
        delete registryFunctionLocale;
        delete registryVariable;
    }

    Class() : registryFunctionLocale(nullptr), registryVariable(nullptr), vtable(nullptr), structType(nullptr), parentHeritage(nullptr) {}

    void setStructType(llvm::StructType* type) { structType = type; }
    void setParentHeritage(INode* node) { parentHeritage = node; }
    void setRegistryVariable(RegistryVariable* reg) { registryVariable = reg; }
    void setRegistryFunctionLocale(RegistryFunctionLocale* reg) { registryFunctionLocale = reg; }
    void setVTable(llvm::GlobalVariable* vtablePtr) { vtable = vtablePtr; }

    // Getters
    [[nodiscard]] auto getRegistryFunctionLocale() const -> RegistryFunctionLocale* { return registryFunctionLocale; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getVTable() const -> llvm::GlobalVariable* { return vtable; }
    [[nodiscard]] auto getStructType() const -> llvm::StructType* { return structType; }
    [[nodiscard]] auto getParentHeritage() const -> INode* { return parentHeritage; }
    [[nodiscard]] auto getMemberIndices() -> std::map<std::string, unsigned int>& { return memberIndices; }
    [[nodiscard]] auto getMemberInitializers() -> std::map<std::string, INode*>& { return memberInitializers; }
    [[nodiscard]] auto getMethodIndices() -> std::map<std::string, unsigned int>& { return methodIndices; }
    
    [[nodiscard]] auto getIndexMethode(const std::string& nomMethode) const -> int {
        auto iterator = methodIndices.find(nomMethode);
        if (iterator != methodIndices.end()) {
            return static_cast<int>(iterator->second);
        }
        return -1;
    }
};
// TODO : supporter les mutex multi thread pour les classes Sinon le multi fichier n'est pas possible

#include <memory>

class RegistryClass : public RegistryGeneric<std::string, std::unique_ptr<Class>>
{
public:
    RegistryClass() = default;
    RegistryClass(const RegistryClass&) = delete;
    auto operator=(const RegistryClass&) -> RegistryClass& = delete;
    RegistryClass(RegistryClass&&) = delete;
    auto operator=(RegistryClass&&) -> RegistryClass& = delete;

    ~RegistryClass() override = default;
};


#endif /* C2ADEE91_A0DA_4404_8AF5_5B1105A499EC */
