#ifndef C51F749C_5233_4BEA_8B77_BFA40834AC90
#define C51F749C_5233_4BEA_8B77_BFA40834AC90

#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <vector>
#include <memory>


class ParseurInstructionAppel : public IParseur, public ParseurBase
{
private:
    IConstructeurArbre* _constructeurArbreEquation;
public:
    ParseurInstructionAppel(IConstructeurArbre* constructeurArbreEquation);
    ~ParseurInstructionAppel();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index) override;
};




#endif /* C51F749C_5233_4BEA_8B77_BFA40834AC90 */
