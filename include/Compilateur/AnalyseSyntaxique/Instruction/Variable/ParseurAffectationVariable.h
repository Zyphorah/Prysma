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
    LlvmBackend* _backend;
    RegistreVariable* _registreVariable;
    RegistreType* _registreType;
    IConstructeurArbre* _constructeurEquation;

public:

    ParseurAffectationVariable(LlvmBackend* backend, RegistreVariable* registreVariable, RegistreType* registreType, IConstructeurArbre* constructeurEquation);
    ~ParseurAffectationVariable();

    INoeud* parser(std::vector<Token>& tokens, int& index) override;
};

#endif /* E5F6G7H8_I9J0K1L2_M3N4O5P6_Q7R8S9T0 */
