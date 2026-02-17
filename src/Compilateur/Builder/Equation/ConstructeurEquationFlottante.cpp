#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include "Compilateur/Lexer/TokenType.h"

std::shared_ptr<RegistreStrategieEquation> ConstructeurEquationFlottante::_registreStrategieEquation = nullptr;

ConstructeurEquationFlottante::ConstructeurEquationFlottante(IConstructeurArbre* instructionBuilder)
    : _instructionBuilder(instructionBuilder)
{
    _registreSymbole = std::make_shared<RegistreSymbole>();

    _serviceParenthese = std::make_unique<ServiceParenthese>(_registreSymbole);

    _gestionnaireAddition = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS);
    _gestionnaireSoustraction = std::make_unique<GestionnaireOperateur>(TOKEN_MOINS);
    _gestionnaireMultiplication = std::make_unique<GestionnaireOperateur>(TOKEN_ETOILE);
    _gestionnaireDivision = std::make_unique<GestionnaireOperateur>(TOKEN_SLASH);
    _gestionnaireModulo = std::make_unique<GestionnaireOperateur>(TOKEN_MODULO);
    _gestionnairePlusPetit = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS_PETIT);
    _gestionnairePlusGrand = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS_GRAND);
    _gestionnairePlusPetitEgal = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS_PETIT_EGAL);
    _gestionnairePlusGrandEgal = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS_GRAND_EGAL);
    _gestionnaireEgal = std::make_unique<GestionnaireOperateur>(TOKEN_EGAL_EGAL);
    _gestionnaireDifferent = std::make_unique<GestionnaireOperateur>(TOKEN_DIFFERENT);
    _gestionnaireEt = std::make_unique<GestionnaireOperateur>(TOKEN_ET);
    _gestionnaireOu = std::make_unique<GestionnaireOperateur>(TOKEN_OU);

    std::vector<GestionnaireOperateur*> operateurs = {
        _gestionnaireOu.get(),
        _gestionnaireEt.get(),
        _gestionnaireEgal.get(),
        _gestionnaireDifferent.get(),
        _gestionnairePlusPetit.get(),
        _gestionnairePlusGrand.get(),
        _gestionnairePlusPetitEgal.get(),
        _gestionnairePlusGrandEgal.get(),
        _gestionnaireAddition.get(), 
        _gestionnaireSoustraction.get(), 
        _gestionnaireMultiplication.get(), 
        _gestionnaireDivision.get(),
        _gestionnaireModulo.get()
    };
            
    _chaineResponsabilite = std::make_unique<ChaineResponsabilite>(_serviceParenthese.get(), operateurs);
                    
    _constructeurArbre = std::make_shared<ConstructeurArbreEquation>(
        _chaineResponsabilite.get(), 
        _registreSymbole, 
        _registreStrategieEquation,
        _serviceParenthese.get(),
        instructionBuilder
    );

    initialiserRegistre();
}

void ConstructeurEquationFlottante::initialiserRegistre()
{
    _registreSymbole->enregistrer(TOKEN_PLUS, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });

    _registreSymbole->enregistrer(TOKEN_MOINS, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });

    _registreSymbole->enregistrer(TOKEN_ETOILE, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });

    _registreSymbole->enregistrer(TOKEN_SLASH, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_PETIT, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_GRAND, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_GRAND_EGAL, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_PETIT_EGAL, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token);
     });
    _registreSymbole->enregistrer(TOKEN_MODULO, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token);
    });
    _registreSymbole->enregistrer(TOKEN_EGAL_EGAL, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token);
    });
    _registreSymbole->enregistrer(TOKEN_DIFFERENT, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token);
    });
    _registreSymbole->enregistrer(TOKEN_ET, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token);
    });
    _registreSymbole->enregistrer(TOKEN_OU, [](Token token) -> std::shared_ptr<IExpression> { 
        return std::make_shared<NoeudOperation>(token);
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

void ConstructeurEquationFlottante::setRegistreStrategieEquation(std::shared_ptr<RegistreStrategieEquation> registre)
{
    _registreStrategieEquation = std::move(registre);
}

std::shared_ptr<RegistreStrategieEquation> ConstructeurEquationFlottante::getRegistreStrategieEquation()
{
    return _registreStrategieEquation;
}
