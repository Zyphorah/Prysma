#ifndef F4099BCE_4896_40B4_B34C_93B4827706C3
#define F4099BCE_4896_40B4_B34C_93B4827706C3

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include <stdexcept>

struct ContextParseur
{
private:
    IConstructeurArbre* constructeurArbreEquation;
    IConstructeurArbre* constructeurArbreInstruction;
    ParseurType* parseurType;
    RegistreVariable* registreVariable;
    RegistreType* registreType;

public:
    ContextParseur(IConstructeurArbre* p_constructeurArbreEquation, IConstructeurArbre* p_constructeurArbreInstruction, ParseurType* p_parseurType,  RegistreVariable* p_registreVariable, RegistreType* p_registreType) // NOLINT(bugprone-easily-swappable-parameters)
        : constructeurArbreEquation(p_constructeurArbreEquation), constructeurArbreInstruction(p_constructeurArbreInstruction), parseurType(p_parseurType),  registreVariable(p_registreVariable), registreType(p_registreType)
    {
        if(p_constructeurArbreEquation == nullptr)
        {
            throw std::invalid_argument("constructeurArbreEquation ne peut pas être nul");
        }

        if(p_constructeurArbreInstruction == nullptr)
        {
            throw std::invalid_argument("constructeurArbreInstruction ne peut pas être nul");
        }

        if(p_parseurType == nullptr)
        {
            throw std::invalid_argument("parseurType ne peut pas être nul");
        }

        if(p_registreVariable == nullptr)
        {
            throw std::invalid_argument("registreVariable ne peut pas être nul");
        }

        if(p_registreType == nullptr)
        {
            throw std::invalid_argument("registreType ne peut pas être nul");
        }
    }    

    [[nodiscard]] auto getConstructeurArbreEquation() const -> IConstructeurArbre* { return constructeurArbreEquation; }
    [[nodiscard]] auto getConstructeurArbreInstruction() const -> IConstructeurArbre* { return constructeurArbreInstruction; }
    [[nodiscard]] auto getParseurType() const -> ParseurType* { return parseurType; }
    [[nodiscard]] auto getRegistreVariable() const -> RegistreVariable* { return registreVariable; }
    [[nodiscard]] auto getRegistreType() const -> RegistreType* { return registreType; }
};

#endif /* F4099BCE_4896_40B4_B34C_93B4827706C3 */
