#ifndef PARSEUR_APPELCENTRAL_H
#define PARSEUR_APPELCENTRAL_H

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class ParseurAppelCentral : public IParseur
{
private:
    ContextParseur& _contextParseur;

public:
    explicit ParseurAppelCentral(ContextParseur& contextParseur);
    ~ParseurAppelCentral() override;

    ParseurAppelCentral(const ParseurAppelCentral&) = delete;
    auto operator=(const ParseurAppelCentral&) -> ParseurAppelCentral& = delete;
    ParseurAppelCentral(ParseurAppelCentral&&) = delete;
    auto operator=(ParseurAppelCentral&&) -> ParseurAppelCentral& = delete;
    
    auto parser(std::vector<Token>& tokens, int& index) -> INoeud* override;
};

#endif /* PARSEUR_APPELCENTRAL_H */