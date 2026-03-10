#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"

ConstructeurEquationFlottante::ConstructeurEquationFlottante(IConstructeurArbre* instructionBuilder, RegistreExpression* registreExpression, llvm::BumpPtrAllocator& arena)
    : _instructionBuilder(instructionBuilder), _arena(arena), _registreExpression(registreExpression)
{
    _registreSymbole = new (_arena.Allocate(sizeof(RegistreSymbole), alignof(RegistreSymbole))) RegistreSymbole();

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
                    
    _constructeurArbre = new (_arena) ConstructeurArbreEquation(
        _chaineResponsabilite.get(), 
        _registreSymbole, 
    _registreExpression,
        _serviceParenthese.get(),
        _arena,
        instructionBuilder
    );

    initialiserRegistre();
}

void ConstructeurEquationFlottante::initialiserRegistre()
{
    _registreSymbole->enregistrer(TOKEN_PLUS, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });

    _registreSymbole->enregistrer(TOKEN_MOINS, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });

    _registreSymbole->enregistrer(TOKEN_ETOILE, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });

    _registreSymbole->enregistrer(TOKEN_SLASH, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_PETIT, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_GRAND, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_GRAND_EGAL, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token)); 
    });
    _registreSymbole->enregistrer(TOKEN_PLUS_PETIT_EGAL, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token));
    });
    _registreSymbole->enregistrer(TOKEN_MODULO, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token));
    });
    _registreSymbole->enregistrer(TOKEN_EGAL_EGAL, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token));
    });
    _registreSymbole->enregistrer(TOKEN_DIFFERENT, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token));
    });
    _registreSymbole->enregistrer(TOKEN_ET, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token));
    });
    _registreSymbole->enregistrer(TOKEN_OU, [this](Token token) -> IExpression* { 
        return new (_arena.Allocate(sizeof(NoeudOperation), alignof(NoeudOperation))) NoeudOperation(std::move(token));
    });
}



INoeud* ConstructeurEquationFlottante::construire(std::vector<Token> &tokens)
{
    return _constructeurArbre->construire(tokens);
}

IConstructeurArbre* ConstructeurEquationFlottante::recupererConstructeurArbre() const
{
    return _constructeurArbre;
}

INoeud* ConstructeurEquationFlottante::construire(std::vector<Token>& tokens, int& index)
{
    return _constructeurArbre->construire(tokens, index);
}

llvm::BumpPtrAllocator& ConstructeurEquationFlottante::getArena()
{
    return _arena;
}
