#ifndef C0F69272_DFFB_405E_A894_789467563D41
#define C0F69272_DFFB_405E_A894_789467563D41

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"

class StrategieLitteral : public IStrategieEquation {
public:
    StrategieLitteral() = default;
    ~StrategieLitteral() override = default;
    std::shared_ptr<INoeud> construire(std::vector<Token>& equation) override;
};


#endif /* C0F69272_DFFB_405E_A894_789467563D41 */
