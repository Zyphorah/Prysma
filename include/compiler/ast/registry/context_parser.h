#ifndef F4099BCE_4896_40B4_B34C_93B4827706C3
#define F4099BCE_4896_40B4_B34C_93B4827706C3

#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/ast/registry/registry_type.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/parser/parser_type.h"
#include <stdexcept>

struct ContextParser
{
private:
    IBuilderTree* builderTreeEquation;
    IBuilderTree* builderTreeInstruction;
    TypeParser* parserType;
    RegistryVariable* registryVariable;
    RegistryType* registryType;
    NodeComponentRegistry* nodeComponentRegistry;

public:
    struct Dependencies {
        IBuilderTree* builderTreeEquation;
        IBuilderTree* builderTreeInstruction;
        TypeParser* parserType;
        RegistryVariable* registryVariable;
        RegistryType* registryType;
        NodeComponentRegistry* nodeComponentRegistry;
    };

    explicit ContextParser(const Dependencies& deps)
        : builderTreeEquation(deps.builderTreeEquation),
          builderTreeInstruction(deps.builderTreeInstruction),
          parserType(deps.parserType),
          registryVariable(deps.registryVariable),
          registryType(deps.registryType),
          nodeComponentRegistry(deps.nodeComponentRegistry)
    {
        if(builderTreeEquation == nullptr)
        {
            throw std::invalid_argument("builderTreeEquation cannot be null");
        }

        if(builderTreeInstruction == nullptr)
        {
            throw std::invalid_argument("builderTreeInstruction cannot be null");
        }

        if(parserType == nullptr)
        {
            throw std::invalid_argument("parserType cannot be null");
        }

        if(registryVariable == nullptr)
        {
            throw std::invalid_argument("registryVariable cannot be null");
        }

        if(registryType == nullptr)
        {
            throw std::invalid_argument("registryType cannot be null");
        }

        if(nodeComponentRegistry == nullptr)
        {
            throw std::invalid_argument("nodeComponentRegistry cannot be null");
        }
    }    

    [[nodiscard]] auto getBuilderTreeEquation() const -> IBuilderTree* { return builderTreeEquation; }
    [[nodiscard]] auto getBuilderTreeInstruction() const -> IBuilderTree* { return builderTreeInstruction; }
    [[nodiscard]] auto getTypeParser() const -> TypeParser* { return parserType; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return registryType; }
    [[nodiscard]] auto getNodeComponentRegistry() const -> NodeComponentRegistry* { return nodeComponentRegistry; }
};

#endif /* F4099BCE_4896_40B4_B34C_93B4827706C3 */
