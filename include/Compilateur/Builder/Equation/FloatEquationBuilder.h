#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/ServiceParenthese.h"
#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"
#include "Compilateur/AST/Noeuds/Operande/Operation.h"
#include <llvm-18/llvm/IR/IRBuilder.h>
#include <memory>

class FloatEquationBuilder
{
    private:

    llvm::IRBuilder<> builder;

    std::shared_ptr<RegistreSymbole> registreSymboleFloat;

    std::unique_ptr<ChaineResponsabilite> chaineResponsabilite;
 
    std::unique_ptr<GestionnaireOperateur> gestionnaireAddition;
    std::unique_ptr<GestionnaireOperateur> gestionnaireSoustraction;
    std::unique_ptr<GestionnaireOperateur> gestionnaireMultiplication;
    std::unique_ptr<GestionnaireOperateur> gestionnaireDivision;

    std::unique_ptr<ServiceParenthese> serviceParenthese;
        
    std::unique_ptr<ConstructeurArbreEquation> constructeurArbreEquation;
    
    void construireRegistreSymboleFloat();
  
    public: 

    FloatEquationBuilder(llvm::LLVMContext &context) : builder(context)
    {
        registreSymboleFloat = std::make_shared<RegistreSymbole>();

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
        constructeurArbreEquation = std::make_unique<ConstructeurArbreEquation>(
            chaineResponsabilite.get(), registreSymboleFloat, serviceParenthese.get(), context
        );

        construireRegistreSymboleFloat();
    }
    
    ~FloatEquationBuilder() = default;
    
    shared_ptr<INoeud> builderArbreEquationFloat(vector<Token> &tokens);
};