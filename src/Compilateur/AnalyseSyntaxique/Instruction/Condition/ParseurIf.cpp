#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
ParseurIf::ParseurIf()
{}

ParseurIf::~ParseurIf()
{}

std::shared_ptr<INoeud> ParseurIf::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) 
{
    std::shared_ptr<NoeudIf> noeudIf = std::make_shared<NoeudIf>();

    consommer(tokens,index,TOKEN_SI,"Erreur, le token n'est pas 'if'! ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
    
    std::shared_ptr<ConstructeurEquationFlottante> constructeurArbreEquation = std::make_shared<ConstructeurEquationFlottante>(constructeurArbre);
    std::shared_ptr<INoeud> condition = constructeurArbreEquation->recupererConstructeurArbre()->construire(tokens, index);
    noeudIf->ajouterInstruction(condition);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudIf,constructeurArbre,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");

    return noeudIf;
}