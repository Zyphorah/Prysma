#ifndef E5F6G7H8_I9J0K1L2_M3N4O5P6_Q7R8S9T0
#define E5F6G7H8_I9J0K1L2_M3N4O5P6_Q7R8S9T0

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include <memory>

class ParseurAffectationVariable : public IParseur, public ParseurBase
{
private:
    std::shared_ptr<LlvmBackend> _backend;
    std::shared_ptr<RegistreVariable> _registreVariable;
    std::shared_ptr<RegistreType> _registreType;
    IConstructeurArbre* _constructeurEquation;

public:

    ParseurAffectationVariable(std::shared_ptr<LlvmBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::shared_ptr<RegistreType> registreType, IConstructeurArbre* constructeurEquation = nullptr);
    ~ParseurAffectationVariable();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;
};

#endif /* E5F6G7H8_I9J0K1L2_M3N4O5P6_Q7R8S9T0 */
