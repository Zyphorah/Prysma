#ifndef A697C19E_AC37_4048_8FF1_CCFCA17CAD98
#define A697C19E_AC37_4048_8FF1_CCFCA17CAD98

#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
class ParseurIf: public IParseur
{
public:
    ParseurIf();
    ~ParseurIf();
    
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;
};


#endif /* A697C19E_AC37_4048_8FF1_CCFCA17CAD98 */
