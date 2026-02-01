#ifndef FLOATEQUATIONBUILDER_H
#define FLOATEQUATIONBUILDER_H

#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/GestionnaireChargementVariable.h"
#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/ServiceParenthese.h"
#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"
#include "Compilateur/AST/Noeuds/Operande/Operation.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>
#include <memory>
#include <utility>

class FloatEquationBuilder
{
    private:

    std::shared_ptr<LLVMBackend> _backend;

    std::shared_ptr<RegistreSymbole> registreSymboleFloat;
    std::shared_ptr<RegistreVariable> registreVariable;

    std::unique_ptr<ChaineResponsabilite> chaineResponsabilite;
 
    std::unique_ptr<GestionnaireOperateur> gestionnaireAddition;
    std::unique_ptr<GestionnaireOperateur> gestionnaireSoustraction;
    std::unique_ptr<GestionnaireOperateur> gestionnaireMultiplication;
    std::unique_ptr<GestionnaireOperateur> gestionnaireDivision;

    std::unique_ptr<ServiceParenthese> serviceParenthese;
        
    std::shared_ptr<IConstructeurArbre> constructeurArbreEquation;
    
    void construireRegistreSymboleFloat();
    
    llvm::Value* chargerVariable(llvm::Value* adresseMemoire, llvm::Type* type, const std::string& nomVariable);
  
    public: 

    FloatEquationBuilder(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariableExterne) 
        : _backend(std::move(backend))
    {
        registreSymboleFloat = std::make_shared<RegistreSymbole>();
        
        // Utiliser le registre externe fourni
        registreVariable = std::move(registreVariableExterne);

        serviceParenthese = std::make_unique<ServiceParenthese>(registreSymboleFloat);

        gestionnaireAddition = std::make_unique<GestionnaireOperateur>(TOKEN_PLUS);
        gestionnaireSoustraction = std::make_unique<GestionnaireOperateur>(TOKEN_MOINS);
        gestionnaireMultiplication = std::make_unique<GestionnaireOperateur>(TOKEN_ETOILE);
        gestionnaireDivision = std::make_unique<GestionnaireOperateur>(TOKEN_SLASH);

        std::vector<GestionnaireOperateur*> operateurs = {
            gestionnaireAddition.get(), gestionnaireSoustraction.get(), 
            gestionnaireMultiplication.get(), gestionnaireDivision.get()
        };
                
        chaineResponsabilite = std::make_unique<ChaineResponsabilite>(serviceParenthese.get(), operateurs);
                        
        // ===== Construction de l'AST et Résolution =====
        shared_ptr<GestionnaireChargementVariable> gestionnaireChargement = std::make_shared<GestionnaireChargementVariable>(_backend);
        constructeurArbreEquation = std::make_shared<ConstructeurArbreEquation>(
            chaineResponsabilite.get(), registreSymboleFloat, serviceParenthese.get(), _backend->getContext(), registreVariable, gestionnaireChargement
        );

        construireRegistreSymboleFloat();
    }
    
    ~FloatEquationBuilder() = default;
    
    std::shared_ptr<INoeud> builderArbreEquationFloat(std::vector<Token> &tokens);
};

#endif /* FLOATEQUATIONBUILDER_H */