#ifndef DDD959AA_55F6_46B7_A433_8E073A1EACCE
#define DDD959AA_55F6_46B7_A433_8E073A1EACCE

#include <memory>
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"

class RegistreVariable;

class ParseurVariable : public IParser, public ParserBase
{
private: 
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registreVariable;

public: 
    ParseurVariable(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable);
    ~ParseurVariable();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override; 
};

#endif /* DDD959AA_55F6_46B7_A433_8E073A1EACCE */
