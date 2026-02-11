#ifndef BA7CC6F6_C206_4FBD_9DFD_D7DD1803CDAD
#define BA7CC6F6_C206_4FBD_9DFD_D7DD1803CDAD
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"

class ParseurUnRefVariable : public IParseur, ParseurBase
{
public: 
    ParseurUnRefVariable();
    ~ParseurUnRefVariable();
    
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;

};

#endif /* BA7CC6F6_C206_4FBD_9DFD_D7DD1803CDAD */
