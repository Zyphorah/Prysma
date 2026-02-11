#ifndef A14406A0_1F90_4136_8CFE_E88047B38B03
#define A14406A0_1F90_4136_8CFE_E88047B38B03

#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
class ParseurElse : public IParseur
{
public:
    ParseurElse();
    ~ParseurElse();
    
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;
};

#endif /* A14406A0_1F90_4136_8CFE_E88047B38B03 */
