#ifndef DD335087_6EDE_4036_872C_8BD586E2625B
#define DD335087_6EDE_4036_872C_8BD586E2625B
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include <memory>

class RegistreVariable;

class ParseurDeclarationFonction : public IParseur, public ParseurBase
{
private:
    IConstructeurArbre* _constructeurArbreInstruction;
    ParseurType* _parseurType;

public:

     ParseurDeclarationFonction(IConstructeurArbre* constructeurArbreInstruction, ParseurType* parseurType);
    ~ParseurDeclarationFonction();

    INoeud* parser(std::vector<Token>& tokens, int& index) override;
};

#endif /* DD335087_6EDE_4036_872C_8BD586E2625B */
