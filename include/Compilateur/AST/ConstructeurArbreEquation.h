#pragma once

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"
#include "Compilateur/AST/Registre/RegistreStrategieEquation.h"
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
    RegistreStrategieEquation* _registreStrategieEquation;
    IGestionnaireParenthese* _gestionnaireParenthese;
    IConstructeurArbre* _instructionBuilder;

public:
  
    ConstructeurArbreEquation(
        ChaineResponsabilite* chaineResponsabilite,
        RegistreSymbole* registreSymbole,
        RegistreStrategieEquation* registreStrategieEquation,
        IGestionnaireParenthese* gestionnaireParenthese,
        IConstructeurArbre* instructionBuilder = nullptr
    );
    
    INoeud* construire(std::vector<Token> &equation) override;
    INoeud* construire(std::vector<Token>& tokens, int& index) override;
};
