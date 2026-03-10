#ifndef FLOATEQUATIONBUILDER_H
#define FLOATEQUATIONBUILDER_H

#include <memory>
#include <vector>

#include "Compilateur/AST/ConstructeurArbreEquation.h"
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
    RegistreSymbole* _registreSymbole;
    
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
    IConstructeurArbre* _instructionBuilder;
    llvm::BumpPtrAllocator& _arena;

    RegistreExpression* _registreExpression;

    void initialiserRegistre();

public: 

    ConstructeurEquationFlottante(IConstructeurArbre* instructionBuilder, RegistreExpression* registreExpression, llvm::BumpPtrAllocator& arena);
    
    ~ConstructeurEquationFlottante() = default;

    INoeud* construire(std::vector<Token>& tokens) override;
    INoeud* construire(std::vector<Token>& tokens, int& index) override;
    llvm::BumpPtrAllocator& getArena() override;
    
    IConstructeurArbre* recupererConstructeurArbre() const;
};

#endif /* FLOATEQUATIONBUILDER_H */