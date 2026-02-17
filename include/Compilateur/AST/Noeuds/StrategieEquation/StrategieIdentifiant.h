#ifndef A4F0FD59_F1AF_4A40_9B9B_C2C5C0BED193
#define A4F0FD59_F1AF_4A40_9B9B_C2C5C0BED193

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"

class StrategieIdentifiant : public IStrategieEquation {
public:
    StrategieIdentifiant() = default;
    ~StrategieIdentifiant() override = default;
    std::shared_ptr<INoeud> construire(std::vector<Token>& equation) override;
};


#endif /* A4F0FD59_F1AF_4A40_9B9B_C2C5C0BED193 */
