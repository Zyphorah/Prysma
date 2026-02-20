#ifndef A4F0FD59_F1AF_4A40_9B9B_C2C5C0BED193
#define A4F0FD59_F1AF_4A40_9B9B_C2C5C0BED193

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"
#include <memory>

class StrategieIdentifiant : public IStrategieEquation {
private:
   IConstructeurArbre* _constructeurArbre;
public:
    StrategieIdentifiant(IConstructeurArbre* constructeurArbre);
    ~StrategieIdentifiant();
    INoeud* construire(std::vector<Token>& equation) override;
};


#endif /* A4F0FD59_F1AF_4A40_9B9B_C2C5C0BED193 */
