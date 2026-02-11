#ifndef B863FD2E_67EB_4E4F_BF6B_F5DEBEF317C9
#define B863FD2E_67EB_4E4F_BF6B_F5DEBEF317C9

#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"

class ParseurScope : public IParseur, public ParseurBase
{
public:
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;

};

#endif /* B863FD2E_67EB_4E4F_BF6B_F5DEBEF317C9 */
