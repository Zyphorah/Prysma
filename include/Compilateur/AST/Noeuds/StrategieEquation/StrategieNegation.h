#ifndef EDA0D60F_5385_4A1B_A8D1_DCBEC90DEC63
#define EDA0D60F_5385_4A1B_A8D1_DCBEC90DEC63

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"

class StrategieNegation : public IStrategieEquation {
public:
    StrategieNegation() = default;
    ~StrategieNegation() override = default;
    std::shared_ptr<INoeud> construire(std::vector<Token>& equation) override;
};


#endif /* EDA0D60F_5385_4A1B_A8D1_DCBEC90DEC63 */
