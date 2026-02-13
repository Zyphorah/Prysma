#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
ParseurIf::ParseurIf()
{}

ParseurIf::~ParseurIf()
{}

std::shared_ptr<INoeud> ParseurIf::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) 
{
    consommer(tokens,index,TOKEN_SI,"Erreur, le token n'est pas 'if'! ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
    
    std::shared_ptr<ConstructeurEquationFlottante> constructeurArbreEquation = std::make_shared<ConstructeurEquationFlottante>(constructeurArbre);
    std::shared_ptr<INoeud> condition = constructeurArbreEquation->recupererConstructeurArbre()->construire(tokens, index);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

    // Créer le noeud bloc IF
    std::shared_ptr<NoeudInstruction> noeudBlocIf = std::make_shared<NoeudInstruction>();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudBlocIf,constructeurArbre,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");

    // Créer le noeud bloc ELSE s'il existe
    std::shared_ptr<NoeudInstruction> noeudBlocElse;
    if (index < (int)tokens.size() && tokens[index].type == TOKEN_SINON) {
        consommer(tokens,index,TOKEN_SINON,"Erreur, le token n'est pas 'else'! ");
        noeudBlocElse = std::make_shared<NoeudInstruction>();
        consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
        consommerEnfantCorps(tokens,index,noeudBlocElse,constructeurArbre,TOKEN_ACCOLADE_FERMEE);
        consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");
    }

    // Créer le noeud bloc ENDIF
    std::shared_ptr<NoeudInstruction> noeudBlocEndif = std::make_shared<NoeudInstruction>();

    std::shared_ptr<NoeudIf> noeudIf = std::make_shared<NoeudIf>(std::move(condition), std::move(noeudBlocIf), std::move(noeudBlocElse), std::move(noeudBlocEndif));

    return noeudIf;
}