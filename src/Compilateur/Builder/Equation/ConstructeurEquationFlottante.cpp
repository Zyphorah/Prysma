#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"

ConstructeurEquationFlottante::ConstructeurEquationFlottante(IConstructeurArbre* instructionBuilder)
    : _instructionBuilder(instructionBuilder)
{
    _registreSymbole = std::make_shared<RegistreSymbole>();

    _serviceParenthese = std::make_unique<ServiceParenthese>(_registreSymbole);

    _gestionnaireAddition = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS);
    _gestionnaireSoustraction = std::make_unique<GestionnaireOperateur>(TOKEN_MOINS);
    _gestionnaireMultiplication = std::make_unique<GestionnaireOperateur>(TOKEN_ETOILE);
    _gestionnaireDivision = std::make_unique<GestionnaireOperateur>(TOKEN_SLASH);

    std::vector<GestionnaireOperateur*> operateurs = {
        _gestionnaireAddition.get(), 
        _gestionnaireSoustraction.get(), 
        _gestionnaireMultiplication.get(), 
        _gestionnaireDivision.get()
    };
            
    _chaineResponsabilite = std::make_unique<ChaineResponsabilite>(_serviceParenthese.get(), operateurs);
                    
    _constructeurArbre = std::make_shared<ConstructeurArbreEquation>(
        _chaineResponsabilite.get(), 
        _registreSymbole, 
        _serviceParenthese.get(),
        instructionBuilder
    );

    initialiserRegistre();
}

void ConstructeurEquationFlottante::initialiserRegistre()
{
    _registreSymbole->enregistrer(TOKEN_PLUS, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_ADDITION); 
    });

    _registreSymbole->enregistrer(TOKEN_MOINS, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_SOUSTRACTION); 
    });

    _registreSymbole->enregistrer(TOKEN_ETOILE, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_MULTIPLICATION); 
    });

    _registreSymbole->enregistrer(TOKEN_SLASH, []() -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(OP_DIVISION); 
    });
}

std::shared_ptr<INoeud> ConstructeurEquationFlottante::construire(std::vector<Token> &tokens)
{
    return _constructeurArbre->construire(tokens);
}

IConstructeurArbre* ConstructeurEquationFlottante::recupererConstructeurArbre() const
{
    return _constructeurArbre.get();
}
