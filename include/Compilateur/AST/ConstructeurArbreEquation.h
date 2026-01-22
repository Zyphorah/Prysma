#pragma once

#include "Noeuds/Interfaces/IExpression.h"
#include "Noeuds/Operande/RegistreSymbole.h"
#include "Parsing/Equation/ChaineResponsabilite.h"
#include "Parsing/Equation/Interfaces/IGestionnaireParenthese.h"
#include <llvm/IR/LLVMContext.h>
#include <memory>
#include <string>

/**
 * @class ConstructeurArbreEquation
 * @brief Constructeur d'arbre d'expression syntaxique
 * Transforme une chaîne en arbre d'expressions récursif
 */
class ConstructeurArbreEquation {
private:
    ChaineResponsabilite* _chaineResponsabilite;
    std::shared_ptr<RegistreSymbole> _registreSymbole;
    IGestionnaireParenthese* _gestionnaireParenthese;
    llvm::LLVMContext& _context;

public:
    /**
     * @brief Constructeur
     * @param chaineResponsabilite Chaîne de responsabilité des opérateurs
     * @param registreSymbole Registre des symboles opérateurs
     * @param gestionnaireParenthese Gestionnaire de gestion des parenthèses
     * @param context Le contexte LLVM pour créer les valeurs
     */
    ConstructeurArbreEquation(
        ChaineResponsabilite* chaineResponsabilite,
        std::shared_ptr<RegistreSymbole> registreSymbole,
        IGestionnaireParenthese* gestionnaireParenthese,
        llvm::LLVMContext& context
    );
    
    /**
     * @brief Construit un arbre d'expression à partir d'une chaîne
     * @param equation L'équation sous forme de chaîne
     * @return Un shared_ptr vers l'expression racine
     */
    std::shared_ptr<IExpression> construire(std::string equation);
};
