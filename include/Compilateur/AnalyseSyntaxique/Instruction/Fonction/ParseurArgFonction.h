#ifndef BDC43121_B136_4DA5_86FF_DCA70967CB37
#define BDC43121_B136_4DA5_86FF_DCA70967CB37
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <memory>

class ParseurArgFonction : public IParseur, public ParseurBase
{
private:
    ParseurType* _constructeurType;
public: 

    ParseurArgFonction(ParseurType* constructeurType);
    ~ParseurArgFonction();
    
    INoeud* parser(std::vector<Token>& tokens, int& index) override;

};

#endif /* BDC43121_B136_4DA5_86FF_DCA70967CB37 */
