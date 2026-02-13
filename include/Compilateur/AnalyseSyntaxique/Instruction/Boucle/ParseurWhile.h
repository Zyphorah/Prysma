#ifndef A244E5E7_BE77_4CB6_A61A_9BF8B1F16AEE
#define A244E5E7_BE77_4CB6_A61A_9BF8B1F16AEE
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"

class ParseurWhile : public IParseur, ParseurBase
{

public: 
    ParseurWhile();
    ~ParseurWhile();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;
};

#endif /* A244E5E7_BE77_4CB6_A61A_9BF8B1F16AEE */
