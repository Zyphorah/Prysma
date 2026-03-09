#ifndef A9AC423F_7A3A_4A2D_9B2F_FE9EA55D9D7A
#define A9AC423F_7A3A_4A2D_9B2F_FE9EA55D9D7A

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"

class StrategieAppelFonction : public IStrategieEquation, public ParseurBase {

private:
    ContextParseur& _contextParseur;

public:

    StrategieAppelFonction(ContextParseur& contextParseur);
    ~StrategieAppelFonction() override = default;
    INoeud* construire(std::vector<Token>& equation) override;
};

#endif /* A9AC423F_7A3A_4A2D_9B2F_FE9EA55D9D7A */
