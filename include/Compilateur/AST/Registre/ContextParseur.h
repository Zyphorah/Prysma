#ifndef F4099BCE_4896_40B4_B34C_93B4827706C3
#define F4099BCE_4896_40B4_B34C_93B4827706C3

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"

struct ContextParseur
{
    IConstructeurArbre* constructeurArbreEquation;
    IConstructeurArbre* constructeurArbreInstruction;
    ParseurType* parseurType;
    llvm::BumpPtrAllocator& arena;
    RegistreVariable* registreVariable;
    RegistreType* registreType;
    
    ContextParseur(IConstructeurArbre* p_constructeurArbreEquation, IConstructeurArbre* p_constructeurArbreInstruction, ParseurType* p_parseurType, llvm::BumpPtrAllocator& p_arena,  RegistreVariable* p_registreVariable, RegistreType* p_registreType)
        : constructeurArbreEquation(p_constructeurArbreEquation), constructeurArbreInstruction(p_constructeurArbreInstruction), parseurType(p_parseurType), arena(p_arena),  registreVariable(p_registreVariable), registreType(p_registreType)
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
};

#endif /* F4099BCE_4896_40B4_B34C_93B4827706C3 */
