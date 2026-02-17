#ifndef A4D04758_FBF2_453B_91A4_1C143BCB5638
#define A4D04758_FBF2_453B_91A4_1C143BCB5638

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>

class StrategieRef : public IStrategieEquation {

public:
    StrategieRef() = default;
    ~StrategieRef() override = default;
    std::shared_ptr<INoeud> construire(std::vector<Token>& equation) override;
};


#endif /* A4D04758_FBF2_453B_91A4_1C143BCB5638 */
