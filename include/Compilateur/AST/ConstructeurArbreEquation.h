#pragma once

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/Interfaces/IGestionnaireParenthese.h"
#include <llvm/IR/LLVMContext.h>
#include <memory>
#include <vector>

/**
 * @class ConstructeurArbreEquation
 * @brief Constructeur d'arbre d'expression syntaxique
 * Transforme une chaîne en arbre d'expressions récursif
 */
class ConstructeurArbreEquation : public IConstructeurArbre
{
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
    std::shared_ptr<INoeud> construire(std::vector<Token> &equation) override;
};
