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

struct Token;

class ConstructeurEquationFlottante
{
private:
      std::shared_ptr<RegistreSymbole> _registreSymbole;
    
    std::unique_ptr<GestionnaireOperateur> _gestionnaireAddition;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireSoustraction;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireMultiplication;
    std::unique_ptr<GestionnaireOperateur> _gestionnaireDivision;

    std::unique_ptr<ServiceParenthese> _serviceParenthese;
    std::unique_ptr<ChaineResponsabilite> _chaineResponsabilite;
        
    std::shared_ptr<IConstructeurArbre> _constructeurArbre;
    IConstructeurArbre* _instructionBuilder;

    void initialiserRegistre();

public: 

    ConstructeurEquationFlottante(IConstructeurArbre* instructionBuilder = nullptr);
    
    ~ConstructeurEquationFlottante() = default;
    
    std::shared_ptr<INoeud> construire(std::vector<Token> &tokens);

    IConstructeurArbre* recupererConstructeurArbre() const;
};

#endif /* FLOATEQUATIONBUILDER_H */