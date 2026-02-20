#ifndef BC83F571_6C52_4181_AF89_0CFE23C9EEA1
#define BC83F571_6C52_4181_AF89_0CFE23C9EEA1

#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"

class ParseurRefVariable : public IParseur, ParseurBase
{
public: 
    ParseurRefVariable();
    ~ParseurRefVariable();

    INoeud* parser(std::vector<Token>& tokens, int& index) override;
};

#endif /* BC83F571_6C52_4181_AF89_0CFE23C9EEA1 */
