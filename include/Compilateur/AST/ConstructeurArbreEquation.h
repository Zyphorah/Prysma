#pragma once

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"
#include "Compilateur/AST/Registre/RegistreExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ChaineResponsabilite.h"
#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireParenthese.h"
#include <memory>
#include <vector>


class ConstructeurArbreEquation : public IConstructeurArbre
{
private:
    ChaineResponsabilite* _chaineResponsabilite;
    RegistreSymbole* _registreSymbole;
    RegistreExpression* _registreExpression;
    IGestionnaireParenthese* _gestionnaireParenthese;
    IConstructeurArbre* _instructionBuilder;
    llvm::BumpPtrAllocator& _arena;
    Token _dernierToken;

public:
  
    ConstructeurArbreEquation(
        ChaineResponsabilite* chaineResponsabilite,
        RegistreSymbole* registreSymbole,
    RegistreExpression* registreExpression,
        IGestionnaireParenthese* gestionnaireParenthese,
        llvm::BumpPtrAllocator& arena,
        IConstructeurArbre* instructionBuilder = nullptr
    );
    
    INoeud* construire(std::vector<Token> &equation) override;
    INoeud* construire(std::vector<Token>& tokens, int& index) override;
    llvm::BumpPtrAllocator& getArena() override;
};
