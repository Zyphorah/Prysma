#ifndef D6C839EF_D128_402F_BE44_4C7AB3B74703
#define D6C839EF_D128_402F_BE44_4C7AB3B74703
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"
#include <memory>

class ParserAppelFonction : public IParser, ParserBase
{
private: 
    std::shared_ptr<RegistreFonction> _registreFonction;
    std::shared_ptr<LLVMBackend> _backend;

public:
    ParserAppelFonction(std::shared_ptr<RegistreFonction> registreFonction, std::shared_ptr<LLVMBackend> backend);
    ~ParserAppelFonction();
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;

};

#endif /* D6C839EF_D128_402F_BE44_4C7AB3B74703 */
