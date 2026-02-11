#ifndef DD335087_6EDE_4036_872C_8BD586E2625B
#define DD335087_6EDE_4036_872C_8BD586E2625B
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <memory>

class RegistreVariable;

class ParseurDeclarationFonction : public IParseur, public ParseurBase
{

public:

     ParseurDeclarationFonction();
    ~ParseurDeclarationFonction();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;
};

#endif /* DD335087_6EDE_4036_872C_8BD586E2625B */
