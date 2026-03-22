#ifndef FLOATEQUATIONBUILDER_H
#define FLOATEQUATIONBUILDER_H

#include <llvm/Support/Allocator.h>
#include <memory>
#include <vector>

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ChaineResponsabilite.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ServiceParenthese.h"
#include "Compilateur/AnalyseSyntaxique/Equation/GestionnaireOperateur.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"
#include "Compilateur/AST/Registre/RegistreExpression.h"

struct Token;

class ConstructeurEquationFlottante : public IConstructeurArbre
{
private:
    std::unique_ptr<RegistreSymbole> _registreSymbole;
    
    std::unique_ptr<GestionnaireOperateur> _gestionnaireAddition;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireSoustraction;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireMultiplication;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireDivision;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireModulo;
    std::unique_ptr<GestionnaireOperateur> _gestionnairePlusPetit;
    std::unique_ptr<GestionnaireOperateur> _gestionnairePlusGrand;
    std::unique_ptr<GestionnaireOperateur> _gestionnairePlusPetitEgal;
    std::unique_ptr<GestionnaireOperateur> _gestionnairePlusGrandEgal;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireEgal;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireDifferent;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireEt;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireOu;

    std::unique_ptr<ServiceParenthese> _serviceParenthese;
    std::unique_ptr<ChaineResponsabilite> _chaineResponsabilite;
        
    IConstructeurArbre* _constructeurArbre;
    RegistreExpression* _registreExpression;
    llvm::BumpPtrAllocator& _arena;


    void initialiserRegistre();

public: 

    ConstructeurEquationFlottante(RegistreExpression* registreExpression, llvm::BumpPtrAllocator& arena);
    
    ~ConstructeurEquationFlottante() override;

    ConstructeurEquationFlottante(const ConstructeurEquationFlottante&) = delete;
    auto operator=(const ConstructeurEquationFlottante&) -> ConstructeurEquationFlottante& = delete;
    ConstructeurEquationFlottante(ConstructeurEquationFlottante&&) = delete;
    auto operator=(ConstructeurEquationFlottante&&) -> ConstructeurEquationFlottante& = delete;

    auto construire(std::vector<Token>& tokens) -> INoeud* override;
    auto construire(std::vector<Token>& tokens, int& index) -> INoeud* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;
    
    [[nodiscard]] auto recupererConstructeurArbre() const -> IConstructeurArbre*;
};

#endif /* FLOATEQUATIONBUILDER_H */