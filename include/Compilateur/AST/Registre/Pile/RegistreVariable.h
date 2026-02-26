#ifndef F7F44FF6_77D4_456C_A897_1A52149FDE53
#define F7F44FF6_77D4_456C_A897_1A52149FDE53

#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include <llvm-18/llvm/IR/Value.h>
#include <map>
#include <stack>

namespace llvm { class AllocaInst; }

struct Symbole {
    llvm::Value* adresse;
    IType* type;

    Symbole() : adresse(nullptr), type(nullptr) {}

    Symbole(llvm::Value* pAdresse, IType* pType) : adresse(pAdresse), type(pType) {}
};

class RegistreVariable 
{
private: 
    std::stack<std::map<std::string, Symbole >>  _variables; 

public:
    RegistreVariable();
    
    ~RegistreVariable();
    
    Symbole recupererVariables(const Token& token);

    void enregistrer(
        const Token& token,
        Symbole symbole
    );
    
    void piler();
    void depiler();
    void viderTop();

    std::map<std::string, Symbole>& getGlobalVariables() {
        return _variables.top();
    }
};

#endif /* F7F44FF6_77D4_456C_A897_1A52149FDE53 */
