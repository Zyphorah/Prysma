#ifndef A244E5E7_BE77_4CB6_A61A_9BF8B1F16AEE
#define A244E5E7_BE77_4CB6_A61A_9BF8B1F16AEE
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"

class ParseurWhile : public IParseur, ParseurBase
{
private:
    IConstructeurArbre* _constructeurArbreEquation;
    IConstructeurArbre* _constructeurArbreInstruction;

public: 
    ParseurWhile(IConstructeurArbre* constructeurArbreEquation, IConstructeurArbre* constructeurArbreInstruction);
    ~ParseurWhile();

    INoeud* parser(std::vector<Token>& tokens, int& index) override;
};

#endif /* A244E5E7_BE77_4CB6_A61A_9BF8B1F16AEE */
