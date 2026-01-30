#ifndef DDD959AA_55F6_46B7_A433_8E073A1EACCE
#define DDD959AA_55F6_46B7_A433_8E073A1EACCE

#include <memory>
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"

class ParseurVariableFloat : public IParser
{
private: 
    std::shared_ptr<LLVMBackend> _backend; 

public: 
    ParseurVariableFloat(std::shared_ptr<LLVMBackend> backend);
    ~ParseurVariableFloat();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, std::shared_ptr<ConstructeurArbreInstruction> constructeurArbreInstruction) override; 
};

#endif /* DDD959AA_55F6_46B7_A433_8E073A1EACCE */
