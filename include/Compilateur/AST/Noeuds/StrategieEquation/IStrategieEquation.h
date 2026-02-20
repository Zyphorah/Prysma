#ifndef BE2F625F_62B2_4905_9534_4BDC9D39156A
#define BE2F625F_62B2_4905_9534_4BDC9D39156A

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>
#include <vector>

class IStrategieEquation {
public:
    virtual ~IStrategieEquation() = default;
    virtual INoeud* construire(std::vector<Token>& equation) = 0;
};


#endif /* BE2F625F_62B2_4905_9534_4BDC9D39156A */
