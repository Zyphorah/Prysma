#ifndef A9AC423F_7A3A_4A2D_9B2F_FE9EA55D9D7A
#define A9AC423F_7A3A_4A2D_9B2F_FE9EA55D9D7A

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>

class StrategieAppelFonction : public IStrategieEquation {

private:
    IConstructeurArbre* _constructeurEquation;

public:

    StrategieAppelFonction(IConstructeurArbre* constructeurEquation);
    ~StrategieAppelFonction() override = default;
    std::shared_ptr<INoeud> construire(std::vector<Token>& equation) override;
};

#endif /* A9AC423F_7A3A_4A2D_9B2F_FE9EA55D9D7A */
