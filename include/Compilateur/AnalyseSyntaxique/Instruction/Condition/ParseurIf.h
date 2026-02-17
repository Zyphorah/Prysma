#ifndef A697C19E_AC37_4048_8FF1_CCFCA17CAD98
#define A697C19E_AC37_4048_8FF1_CCFCA17CAD98

#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
class ParseurIf: public IParseur, ParseurBase
{
private:
    IConstructeurArbre* _constructeurArbreEquation;
    IConstructeurArbre* _constructeurArbreInstruction;
public:
    ParseurIf(IConstructeurArbre* constructeurArbreEquation, IConstructeurArbre* constructeurArbreInstruction);
    ~ParseurIf();
    
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index) override;
};


#endif /* A697C19E_AC37_4048_8FF1_CCFCA17CAD98 */
