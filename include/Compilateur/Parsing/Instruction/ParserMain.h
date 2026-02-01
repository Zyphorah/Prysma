#ifndef B863FD2E_67EB_4E4F_BF6B_F5DEBEF317C9
#define B863FD2E_67EB_4E4F_BF6B_F5DEBEF317C9

#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Parsing/ParserBase.h"

class ParserMain : public IParser, public ParserBase
{
public:
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) override;

};

#endif /* B863FD2E_67EB_4E4F_BF6B_F5DEBEF317C9 */
