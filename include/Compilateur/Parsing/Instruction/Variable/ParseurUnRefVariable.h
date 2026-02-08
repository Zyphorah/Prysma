#ifndef BA7CC6F6_C206_4FBD_9DFD_D7DD1803CDAD
#define BA7CC6F6_C206_4FBD_9DFD_D7DD1803CDAD
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"

class ParseurUnRefVariable : public IParser, ParserBase
{
public: 
    ParseurUnRefVariable();
    ~ParseurUnRefVariable();
    
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;

};

#endif /* BA7CC6F6_C206_4FBD_9DFD_D7DD1803CDAD */
