#ifndef BDC43121_B136_4DA5_86FF_DCA70967CB37
#define BDC43121_B136_4DA5_86FF_DCA70967CB37
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"

class ParseurArgFonction : public IParseur, public ParseurBase
{
    
public: 

    ParseurArgFonction();
    ~ParseurArgFonction();
    
    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;

};

#endif /* BDC43121_B136_4DA5_86FF_DCA70967CB37 */
