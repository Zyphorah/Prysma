#ifndef F7F44FF6_77D4_456C_A897_1A52149FDE53
#define F7F44FF6_77D4_456C_A897_1A52149FDE53

#include "Compilateur/Lexer/Lexer.h"
#include <map>
#include <memory>
#include <stack>


namespace llvm { class AllocaInst; }
// Faire une pile de contexte dans le futur, une pile de dictionnaire pour les contextes différents
class RegistreVariable 
{
    private: 
       std::stack<std::map<std::string, shared_ptr<llvm::AllocaInst>>>  _variables; 

    public:
        RegistreVariable();
        
        ~RegistreVariable();
        
        std::shared_ptr<llvm::AllocaInst> recupererVariables(const Token& token);

        void enregistrer(
            const Token& token,
            shared_ptr<llvm::AllocaInst> instance
        );
        void piler();
        void depiler();
};

#endif /* F7F44FF6_77D4_456C_A897_1A52149FDE53 */
