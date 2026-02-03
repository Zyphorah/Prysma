#ifndef F7H8I9J0_K1L2M3N4_O5P6Q7R8_S9T0U1V2
#define F7H8I9J0_K1L2M3N4_O5P6Q7R8_S9T0U1V2

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <memory>

class RegistreVariable;
class RegistreType;

class ParseurDeclaration : public IParser, public ParserBase
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registreVariable;
    std::shared_ptr<RegistreType> _registreType;

public:
    ParseurDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::shared_ptr<RegistreType> registreType);
    ~ParseurDeclaration();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;
};

#endif /* F7H8I9J0_K1L2M3N4_O5P6Q7R8_S9T0U1V2 */
