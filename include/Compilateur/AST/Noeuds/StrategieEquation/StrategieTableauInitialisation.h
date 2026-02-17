#ifndef A4D04758_FBF2_453D_91A4_1C143BCB5638
#define A4D04758_FBF2_453D_91A4_1C143BCB5638

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"

class StrategieTableauInitialisation : public IStrategieEquation {
public:
    StrategieTableauInitialisation() = default;
    ~StrategieTableauInitialisation() override = default;
    std::shared_ptr<INoeud> construire(std::vector<Token>& equation) override;
};

#endif /* A4D04758_FBF2_453D_91A4_1C143BCB5638 */
