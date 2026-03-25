#ifndef F4099BCE_4896_40B4_B34C_93B4827706C3
#define F4099BCE_4896_40B4_B34C_93B4827706C3

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/Parser/ParserType.h"
#include <stdexcept>

struct ContextParser
{
private:
    IBuilderTree* builderTreeEquation;
    IBuilderTree* builderTreeInstruction;
    ParserType* parserType;
    RegistryVariable* registryVariable;
    RegistryType* registryType;

public:
    struct Dependencies {
        IBuilderTree* builderTreeEquation;
        IBuilderTree* builderTreeInstruction;
        ParserType* parserType;
        RegistryVariable* registryVariable;
        RegistryType* registryType;
    };

    explicit ContextParser(const Dependencies& deps)
        : builderTreeEquation(deps.builderTreeEquation),
          builderTreeInstruction(deps.builderTreeInstruction),
          parserType(deps.parserType),
          registryVariable(deps.registryVariable),
          registryType(deps.registryType)
    {
        if(builderTreeEquation == nullptr)
        {
            throw std::invalid_argument("builderTreeEquation ne peut pas être nul");
        }

        if(builderTreeInstruction == nullptr)
        {
            throw std::invalid_argument("builderTreeInstruction ne peut pas être nul");
        }

        if(parserType == nullptr)
        {
            throw std::invalid_argument("parserType ne peut pas être nul");
        }

        if(registryVariable == nullptr)
        {
            throw std::invalid_argument("registryVariable ne peut pas être nul");
        }

        if(registryType == nullptr)
        {
            throw std::invalid_argument("registryType ne peut pas être nul");
        }
    }    

    [[nodiscard]] auto getBuilderTreeEquation() const -> IBuilderTree* { return builderTreeEquation; }
    [[nodiscard]] auto getBuilderTreeInstruction() const -> IBuilderTree* { return builderTreeInstruction; }
    [[nodiscard]] auto getParserType() const -> ParserType* { return parserType; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return registryType; }
};

#endif /* F4099BCE_4896_40B4_B34C_93B4827706C3 */
