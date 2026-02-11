#ifndef F7H8I9J0_K1L2M3N4_O5P6Q7R8_S9T0U1V2
#define F7H8I9J0_K1L2M3N4_O5P6Q7R8_S9T0U1V2

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <memory>

class RegistreVariable;
class RegistreType;

class ParseurDeclarationVariable : public IParseur, public ParseurBase
{

public:

    ParseurDeclarationVariable();
    ~ParseurDeclarationVariable();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;
};

#endif /* F7H8I9J0_K1L2M3N4_O5P6Q7R8_S9T0U1V2 */
