#ifndef A089019F_3756_4ED8_96C6_BBAA2C5A05F0
#define A089019F_3756_4ED8_96C6_BBAA2C5A05F0

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/Parser/ParserType.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include <llvm/Support/Allocator.h>
#include <stdexcept>

struct ContextExpression
{
private:
    IBuilderTree* builderTreeEquation;
    IBuilderTree* builderTreeInstruction;
    ParserType* parserType;
    ContextParser* contextParser;
    llvm::BumpPtrAllocator* arena;
    RegistryVariable* registryVariable;
    RegistryType* registryType;

public:
    ContextExpression(
        IBuilderTree* p_builderTreeEquation,
        IBuilderTree* p_builderTreeInstruction,
        ParserType* p_parserType,
        ContextParser* p_contextParser,
        llvm::BumpPtrAllocator* p_arena,
        RegistryVariable* p_registryVariable,
        RegistryType* p_registryType
    )
        : builderTreeEquation(p_builderTreeEquation),
          builderTreeInstruction(p_builderTreeInstruction),
          parserType(p_parserType),
          contextParser(p_contextParser),
          arena(p_arena),
          registryVariable(p_registryVariable),
          registryType(p_registryType)
    {
        if (p_contextParser == nullptr)
        {
            throw std::invalid_argument("contextParser ne peut pas être nul");
        }
        if (p_builderTreeEquation == nullptr)
        {
            throw std::invalid_argument("builderTreeEquation ne peut pas être nul");
        }
        if (p_builderTreeInstruction == nullptr)
        {
            throw std::invalid_argument("builderTreeInstruction ne peut pas être nul");
        }
        if (p_parserType == nullptr)
        {
            throw std::invalid_argument("parserType ne peut pas être nul");
        }
         if (p_registryVariable == nullptr)
        {
            throw std::invalid_argument("registryVariable ne peut pas être nul");
        }
         if (p_registryType == nullptr)
        {
            throw std::invalid_argument("registryType ne peut pas être nul");
        }   
    }

    [[nodiscard]] auto getBuilderTreeEquation() const -> IBuilderTree* { return builderTreeEquation; }
    [[nodiscard]] auto getBuilderTreeInstruction() const -> IBuilderTree* { return builderTreeInstruction; }
    [[nodiscard]] auto getParserType() const -> ParserType* { return parserType; }
    [[nodiscard]] auto getContextParser() const -> ContextParser* { return contextParser; }
    [[nodiscard]] auto getArena() const -> llvm::BumpPtrAllocator* { return arena; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return registryType; }
    
};

#endif /* A089019F_3756_4ED8_96C6_BBAA2C5A05F0 */