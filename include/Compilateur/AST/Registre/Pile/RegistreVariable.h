#ifndef F7F44FF6_77D4_456C_A897_1A52149FDE53
#define F7F44FF6_77D4_456C_A897_1A52149FDE53

#include "Compilateur/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Value.h>
#include <map>
#include <stack>

namespace llvm { class AllocaInst; }


class RegistreVariable 
{
    private: 
       std::stack<std::map<std::string, llvm::Value* >>  _variables; 

    public:
        RegistreVariable();
        
        ~RegistreVariable();
        
        llvm::Value* recupererVariables(const Token& token);

        void enregistrer(
            const Token& token,
            llvm::Value* instance
        );
        
        void piler();
        void depiler();
};

#endif /* F7F44FF6_77D4_456C_A897_1A52149FDE53 */
