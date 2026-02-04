#ifndef F39730D3_0913_46B1_91E5_676A85D4CABB
#define F39730D3_0913_46B1_91E5_676A85D4CABB

#include "Compilateur/AST/Registre/Pile/ReturnContextCompilation.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"
#include <memory>

class ParsingReturn : public IParser, public ParserBase
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<ReturnContextCompilation> _returnContextCompilation;
    std::shared_ptr<RegistreType> _registreType;
public:
    explicit ParsingReturn(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<ReturnContextCompilation> returnContextCompilation, std::shared_ptr<RegistreType> registreType);
    
    ~ParsingReturn();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;
};

#endif /* F39730D3_0913_46B1_91E5_676A85D4CABB */
