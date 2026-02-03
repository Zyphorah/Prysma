#ifndef F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6
#define F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6

#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include <vector>
#include <memory>

class ParseurEquation : public IParser, public ParserBase
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registreVariable;
    std::unique_ptr<FloatEquationBuilder> _floatEquationBuilder;
    llvm::Type* type; 
  
public:
    ParseurEquation(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable);
    ~ParseurEquation();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;

private:
    void validerEquation(const std::vector<Token>& tokensEquation) const;
    bool estOperateur(TokenType tokenType) const;
    bool estOperande(TokenType tokenType) const;
};

#endif /* F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6 */
