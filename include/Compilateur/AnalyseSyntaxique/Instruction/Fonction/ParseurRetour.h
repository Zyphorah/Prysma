#ifndef F39730D3_0913_46B1_91E5_676A85D4CABB
#define F39730D3_0913_46B1_91E5_676A85D4CABB

#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <memory>

class ParseurRetour : public IParseur, public ParseurBase
{
private:
    IConstructeurArbre* _constructeurEquation;  // For parsing equations

public:

    explicit ParseurRetour(IConstructeurArbre* constructeurEquation = nullptr);
    
    ~ParseurRetour();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;
};

#endif /* F39730D3_0913_46B1_91E5_676A85D4CABB */
