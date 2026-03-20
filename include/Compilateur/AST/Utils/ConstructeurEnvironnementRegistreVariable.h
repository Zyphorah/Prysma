#ifndef A1014A88_A2B2_4637_B4CD_3709FCBE042F
#define A1014A88_A2B2_4637_B4CD_3709FCBE042F

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Utils/IConstructeurEnvironnementRegistre.h"

class ConstructeurEnvironnementRegistreVariable : public IConstructeurEnvironnementRegistre
{
private:
    ContextGenCode* _contextGenCode;

public:
    explicit ConstructeurEnvironnementRegistreVariable(ContextGenCode* _contextGenCode);
    ~ConstructeurEnvironnementRegistreVariable() override;

    ConstructeurEnvironnementRegistreVariable(const ConstructeurEnvironnementRegistreVariable&) = delete;
    auto operator=(const ConstructeurEnvironnementRegistreVariable&) -> ConstructeurEnvironnementRegistreVariable& = delete;

    ConstructeurEnvironnementRegistreVariable(ConstructeurEnvironnementRegistreVariable&&) = delete;
    auto operator=(ConstructeurEnvironnementRegistreVariable&&) -> ConstructeurEnvironnementRegistreVariable& = delete;

    void remplir() override;
};

#endif /* A1014A88_A2B2_4637_B4CD_3709FCBE042F */
