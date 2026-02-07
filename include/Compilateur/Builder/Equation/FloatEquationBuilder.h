#ifndef FLOATEQUATIONBUILDER_H
#define FLOATEQUATIONBUILDER_H

#include <memory>
#include <vector>


#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/ServiceParenthese.h"
#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"

struct Token;

class FloatEquationBuilder
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
    

    void initialiserRegistre();

public: 

    FloatEquationBuilder();
    
    ~FloatEquationBuilder() = default;
    
    std::shared_ptr<INoeud> construire(std::vector<Token> &tokens);
};

#endif /* FLOATEQUATIONBUILDER_H */