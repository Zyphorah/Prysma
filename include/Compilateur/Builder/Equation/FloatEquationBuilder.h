#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/ServiceParenthese.h"
#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"
#include "Compilateur/AST/Noeuds/Operande/Operation.h"
#include <llvm-18/llvm/IR/IRBuilder.h>


class FloatEquationBuilder
{
    private:

    // === LLVM Builder ===
    llvm::IRBuilder<> builder;

    // === registre de symbole ===
    std::shared_ptr<RegistreSymbole> registreSymboleFloat;

    // ==== Chaine de responsabilité ====
    ChaineResponsabilite* chaineResponsabilite;
 
    // ==== Gestionnaire operateur =====
    GestionnaireOperateur* gestionnaireAddition;
    GestionnaireOperateur* gestionnaireSoustraction;
    GestionnaireOperateur* gestionnaireMultiplication;
    GestionnaireOperateur* gestionnaireDivision;
    
    // ===== Chaîne de responsabilité =====
     ServiceParenthese* serviceParenthese;
        
    // ===== Construction de l'AST et Résolution =====
    ConstructeurArbreEquation* constructeurArbreEquation;
    
    void construireRegistreSymboleFloat();
  
    public: 

    FloatEquationBuilder(llvm::LLVMContext &context) : builder(context)
    {

        // === registre de symbole ===
        registreSymboleFloat = std::make_shared<RegistreSymbole>();

        // ===== Chaîne de responsabilité =====
        serviceParenthese = new ServiceParenthese(registreSymboleFloat);

        // ==== Gestionnaire operateur =====
        gestionnaireAddition = new GestionnaireOperateur(TOKEN_PLUS);
        gestionnaireSoustraction = new GestionnaireOperateur(TOKEN_MOINS);
        gestionnaireMultiplication = new GestionnaireOperateur(TOKEN_ETOILE);
        gestionnaireDivision = new GestionnaireOperateur(TOKEN_SLASH);


        std::vector<GestionnaireOperateur*> operateurs = {
            gestionnaireAddition, gestionnaireSoustraction, 
            gestionnaireMultiplication, gestionnaireDivision
        };
                
        chaineResponsabilite = new ChaineResponsabilite(serviceParenthese, operateurs);
                        
        // ===== Construction de l'AST et Résolution =====
        constructeurArbreEquation = new ConstructeurArbreEquation(
            chaineResponsabilite, registreSymboleFloat, serviceParenthese, context
        );
    }
    ~FloatEquationBuilder()
    {
        delete constructeurArbreEquation;
        delete chaineResponsabilite;
        delete gestionnaireAddition; 
        delete gestionnaireSoustraction;
        delete gestionnaireMultiplication; 
        delete gestionnaireDivision;
        delete serviceParenthese;
       
    }
    
    shared_ptr<IExpression> builderArbreEquationFloat(vector<Token> &tokens);
   

};