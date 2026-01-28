#ifndef F7F44FF6_77D4_456C_A897_1A52149FDE53
#define F7F44FF6_77D4_456C_A897_1A52149FDE53

#include "Compilateur/Lexer/Lexer.h"
#include <map>
#include <memory>


namespace llvm { class AllocaInst; }

class RegistreVariable 
{
    private: 
        std::map<std::string, shared_ptr<llvm::AllocaInst>> variables; 

    public:
        RegistreVariable();
        
        ~RegistreVariable();
        
        std::shared_ptr<llvm::AllocaInst> recupererVariables(const Token& token);

        void enregistrer(
            const Token& token,
            shared_ptr<llvm::AllocaInst> instance
        );
};

#endif /* F7F44FF6_77D4_456C_A897_1A52149FDE53 */
