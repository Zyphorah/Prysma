#ifndef A089019F_3756_4ED8_96C6_BBAA2C5A05F0
#define A089019F_3756_4ED8_96C6_BBAA2C5A05F0

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"

struct ContexteExpression
{
    IConstructeurArbre* constructeurArbreEquation;
    IConstructeurArbre* constructeurArbreInstruction;
    ParseurType* parseurType;
    ContextParseur* contextParseur;
    llvm::BumpPtrAllocator& arena;
    RegistreVariable* registreVariable;
    RegistreType* registreType;

    ContexteExpression(
        IConstructeurArbre* p_constructeurArbreEquation,
        IConstructeurArbre* p_constructeurArbreInstruction,
        ParseurType* p_parseurType,
        ContextParseur* p_contextParseur,
        llvm::BumpPtrAllocator& p_arena,
        RegistreVariable* p_registreVariable,
        RegistreType* p_registreType
    )
        : constructeurArbreEquation(p_constructeurArbreEquation),
          constructeurArbreInstruction(p_constructeurArbreInstruction),
          parseurType(p_parseurType),
          contextParseur(p_contextParseur),
          arena(p_arena),
          registreVariable(p_registreVariable),
          registreType(p_registreType)
    {
        if (p_contextParseur == nullptr)
        {
            throw std::invalid_argument("contextParseur ne peut pas être nul");
        }
        if (p_constructeurArbreEquation == nullptr)
        {
            throw std::invalid_argument("constructeurArbreEquation ne peut pas être nul");
        }
        if (p_constructeurArbreInstruction == nullptr)
        {
            throw std::invalid_argument("constructeurArbreInstruction ne peut pas être nul");
        }
        if (p_parseurType == nullptr)
        {
            throw std::invalid_argument("parseurType ne peut pas être nul");
        }
         if (p_registreVariable == nullptr)
        {
            throw std::invalid_argument("registreVariable ne peut pas être nul");
        }
         if (p_registreType == nullptr)
        {
            throw std::invalid_argument("registreType ne peut pas être nul");
        }   
    }
};

#endif /* A089019F_3756_4ED8_96C6_BBAA2C5A05F0 */