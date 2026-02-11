#ifndef F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6
#define F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6

#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <vector>
#include <memory>

class ParseurEquation : public IParseur, public ParseurBase
{
private:
    std::shared_ptr<ConstructeurEquationFlottante> _floatEquationBuilder;
    
    void validerEquation(const std::vector<Token>& tokensEquation) const;
    bool estOperateur(TokenType tokenType) const;
    bool estOperande(TokenType tokenType) const;
    bool estOperateurUnaire(TokenType tokenType) const;
  
public:
    ParseurEquation();
    ~ParseurEquation();

    std::shared_ptr<INoeud> parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) override;
};

#endif /* F3A7B8C9_D4E2_4F6A_B1C3_E5G7H9I2J4K6 */
