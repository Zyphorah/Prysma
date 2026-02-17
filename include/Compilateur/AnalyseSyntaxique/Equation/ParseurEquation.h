#ifndef F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6
#define F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6

#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <vector>
#include <memory>

class ParseurEquation : public IParseur, public ParseurBase
{
    IConstructeurArbre* _constructeurArbre;
public:
    ParseurEquation(IConstructeurArbre* constructeurArbre);
    ~ParseurEquation();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index) override;
};

#endif /* F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6 */
