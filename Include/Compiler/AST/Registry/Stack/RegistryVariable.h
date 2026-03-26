#ifndef F7F44FF6_77D4_456C_A897_1A52149FDE53
#define F7F44FF6_77D4_456C_A897_1A52149FDE53

#include "Compiler/Lexer/Lexer.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <llvm-18/llvm/IR/Value.h>
#include <map>
#include <stack>
#include <string>

namespace llvm { class AllocaInst; }

struct Symbol {
private:
    llvm::Value* address;
    IType* type;
    llvm::Type* pointedElementType;

public:
    Symbol() : address(nullptr), type(nullptr), pointedElementType(nullptr) {}

    Symbol(llvm::Value* pAddress, IType* pType) : address(pAddress), type(pType), pointedElementType(nullptr) {}

    Symbol(llvm::Value* pAddress, IType* pType, llvm::Type* pPointedElementType)
        : address(pAddress), type(pType), pointedElementType(pPointedElementType) {}

    [[nodiscard]] auto getAddress() const -> llvm::Value* { return address; }
    [[nodiscard]] auto getType() const -> IType* { return type; }
    [[nodiscard]] auto getPointedElementType() const -> llvm::Type* { return pointedElementType; }
};

class RegistryVariable 
{
private: 
    std::stack<std::map<std::string, Symbol >>  _variables; 

public:
    RegistryVariable();
    
    ~RegistryVariable();

    RegistryVariable(const RegistryVariable&) = delete;
    auto operator=(const RegistryVariable&) -> RegistryVariable& = delete;
    RegistryVariable(RegistryVariable&&) = delete;
    auto operator=(RegistryVariable&&) -> RegistryVariable& = delete;
    
    auto getVariable(const Token& token) -> Symbol;

    void registerVariable(
        const Token& token,
        Symbol symbol
    );
    
    void push();
    void pop();
    void clearTop();

    auto variableExists(const std::string& name) -> bool;

    auto getGlobalVariables() -> std::map<std::string, Symbol>& {
        return _variables.top();
    }
};

#endif /* F7F44FF6_77D4_456C_A897_1A52149FDE53 */
