#ifndef D6C839EF_D128_402F_BE44_4C7AB3B74703
#define D6C839EF_D128_402F_BE44_4C7AB3B74703
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <memory>

class ParseurAppelFonction : public IParseur, ParseurBase
{

private:
    IConstructeurArbre* _constructeurArbreEquation;
public:

    ParseurAppelFonction(IConstructeurArbre* constructeurArbreEquation);
    ~ParseurAppelFonction();
    INoeud* parser(std::vector<Token>& tokens, int& index) override;

};

#endif /* D6C839EF_D128_402F_BE44_4C7AB3B74703 */
