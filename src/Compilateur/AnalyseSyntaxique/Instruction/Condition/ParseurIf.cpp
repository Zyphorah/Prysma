#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudBlocIf.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudBlocElse.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudBlocEndif.h"
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

    // Créer le noeud bloc IF
    std::shared_ptr<NoeudBlocIf> noeudBlocIf = std::make_shared<NoeudBlocIf>();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudBlocIf,constructeurArbre,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");
    noeudIf->ajouterInstruction(noeudBlocIf);

    // Gérer le bloc ELSE s'il existe
    if (index < (int)tokens.size() && tokens[index].type == TOKEN_SINON) {
        consommer(tokens,index,TOKEN_SINON,"Erreur, le token n'est pas 'else'! ");
        
        // Créer le noeud bloc ELSE
        std::shared_ptr<NoeudBlocElse> noeudBlocElse = std::make_shared<NoeudBlocElse>();
        consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
        consommerEnfantCorps(tokens,index,noeudBlocElse,constructeurArbre,TOKEN_ACCOLADE_FERMEE);
        consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");
        noeudIf->ajouterInstruction(noeudBlocElse);
    }

    // Créer et ajouter le noeud bloc ENDIF
    std::shared_ptr<NoeudBlocEndif> noeudBlocEndif = std::make_shared<NoeudBlocEndif>();
    noeudIf->ajouterInstruction(noeudBlocEndif);

    return noeudIf;
}