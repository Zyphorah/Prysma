#ifndef F7F44FF6_77D4_456C_A897_1A52149FDE53
#define F7F44FF6_77D4_456C_A897_1A52149FDE53

#include "Compiler/Lexer/Lexer.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <llvm-18/llvm/IR/Value.h>
#include <map>
#include <stack>
#include <string>

namespace llvm { class AllocaInst; }

struct Symbole {
private:
    llvm::Value* adresse;
    IType* type;
    llvm::Type* typePointeElement;

public:
    Symbole() : adresse(nullptr), type(nullptr), typePointeElement(nullptr) {}

    Symbole(llvm::Value* pAdresse, IType* pType) : adresse(pAdresse), type(pType), typePointeElement(nullptr) {}

    Symbole(llvm::Value* pAdresse, IType* pType, llvm::Type* pTypePointeElement)
        : adresse(pAdresse), type(pType), typePointeElement(pTypePointeElement) {}

    [[nodiscard]] auto getAdresse() const -> llvm::Value* { return adresse; }
    [[nodiscard]] auto getType() const -> IType* { return type; }
    [[nodiscard]] auto getTypePointeElement() const -> llvm::Type* { return typePointeElement; }
};

class RegistryVariable 
{
private: 
    std::stack<std::map<std::string, Symbole >>  _variables; 

public:
    RegistryVariable();
    
    ~RegistryVariable();

    RegistryVariable(const RegistryVariable&) = delete;
    auto operator=(const RegistryVariable&) -> RegistryVariable& = delete;
    RegistryVariable(RegistryVariable&&) = delete;
    auto operator=(RegistryVariable&&) -> RegistryVariable& = delete;
    
    auto recupererVariables(const Token& token) -> Symbole;

    void enregistryr(
        const Token& token,
        Symbole symbole
    );
    
    void piler();
    void depiler();
    void viderTop();

    auto existeVariable(const std::string& nom) -> bool;

    auto getGlobalVariables() -> std::map<std::string, Symbole>& {
        return _variables.top();
    }
};

#endif /* F7F44FF6_77D4_456C_A897_1A52149FDE53 */
