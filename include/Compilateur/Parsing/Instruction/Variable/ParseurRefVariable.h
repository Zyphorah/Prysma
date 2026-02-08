#ifndef BC83F571_6C52_4181_AF89_0CFE23C9EEA1
#define BC83F571_6C52_4181_AF89_0CFE23C9EEA1

#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"

class ParseurRefVariable : public IParser, ParserBase
{
public: 
    ParseurRefVariable();
    ~ParseurRefVariable();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;
};

#endif /* BC83F571_6C52_4181_AF89_0CFE23C9EEA1 */
