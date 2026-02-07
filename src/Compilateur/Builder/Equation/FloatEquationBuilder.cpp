#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"

void FloatEquationBuilder::construireRegistreSymboleFloat()
{   
    registreSymboleFloat->enregistrer(TOKEN_PLUS, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_ADDITION); 
    });

    registreSymboleFloat->enregistrer(TOKEN_MOINS, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_SOUSTRACTION); 
    });

    registreSymboleFloat->enregistrer(TOKEN_ETOILE, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_MULTIPLICATION); 
    });

    registreSymboleFloat->enregistrer(TOKEN_SLASH, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_DIVISION); 
    });
}

shared_ptr<INoeud> FloatEquationBuilder::builderArbreEquationFloat(vector<Token> &tokens)
{
    return constructeurArbreEquation->construire(tokens);   
}
