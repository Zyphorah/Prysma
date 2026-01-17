#include "AST/Noeuds/Operande/RegistreSymbole.h"
#include "AST/ConstructeurArbreEquation.h"
#include "Parsing/Equation/ChaineResponsabilite.h"
#include "Parsing/Equation/ServiceParenthese.h"
#include "Parsing/Equation/GestionnaireOperateur.h"
#include "AST/Noeuds/Operande/Operation.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    try {
        // ===== Configuration du registre des symboles =====
        // IMPORTANT: L'ordre détermine la priorité d'exécution
        // Enregistré EN PREMIER = priorité BASSE (addition/soustraction)
        // Enregistré EN DERNIER = priorité HAUTE (multiplication/division)
        std::string equation = "3 + 5 * (2 - 8) / 4";

        std::shared_ptr<RegistreSymbole> registreSymbole = std::make_shared<RegistreSymbole>();

        // Enregistrer les opérateurs en ordre de priorité croissante
        registreSymbole->enregistrer('+', []() { 
            return std::make_shared<Operation>([](double a, double b) { return a + b; }); 
        });
        
        registreSymbole->enregistrer('-', []() { 
            return std::make_shared<Operation>([](double a, double b) { return a - b; }); 
        });
        
        registreSymbole->enregistrer('*', []() { 
            return std::make_shared<Operation>([](double a, double b) { return a * b; }); 
        });
        
        registreSymbole->enregistrer('/', []() { 
            return std::make_shared<Operation>([](double a, double b) { 
                if (b == 0) throw std::runtime_error("Division par zéro");
                return a / b; 
            }); 
        });
        
        // ===== Configuration de la chaîne de responsabilité =====
        ServiceParenthese* serviceParenthese = new ServiceParenthese(registreSymbole);
        
        // Créer les gestionnaires en ordre de priorité
        GestionnaireOperateur* gestionnaireAddition = new GestionnaireOperateur('+');
        GestionnaireOperateur* gestionnaireSoustraction = new GestionnaireOperateur('-');
        GestionnaireOperateur* gestionnaireMultiplication = new GestionnaireOperateur('*');
        GestionnaireOperateur* gestionnaireDivision = new GestionnaireOperateur('/');
        
        std::vector<GestionnaireOperateur*> operateurs = {
            gestionnaireAddition,
            gestionnaireSoustraction,
            gestionnaireMultiplication,
            gestionnaireDivision
        };
        
        ChaineResponsabilite* chaineResponsabilite = new ChaineResponsabilite(serviceParenthese, operateurs);
                        
        // ===== Configuration de la substitution et de la construction =====
        ConstructeurArbreEquation* constructeurArbreEquation = new ConstructeurArbreEquation(
            chaineResponsabilite, 
            registreSymbole, 
            serviceParenthese
        );

        std::shared_ptr<IExpression> expression = constructeurArbreEquation->construire(equation);

        // Utiliser l'expression construite
        std::cout << "Expression construite avec succès." << std::endl;
        std::cout << "Résultat de l'expression '" << equation << "' = " << expression->resoudre() << std::endl;
        // ===== Nettoyage =====
        delete constructeurArbreEquation;
        delete chaineResponsabilite;
        delete gestionnaireAddition;
        delete gestionnaireSoustraction;
        delete gestionnaireMultiplication;
        delete gestionnaireDivision;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}
