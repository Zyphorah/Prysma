#ifndef BDC39C44_6952_4793_8198_C083B106A089
#define BDC39C44_6952_4793_8198_C083B106A089

#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/lexer/lexer.h"
#include "compiler/ast/registry/registry_instruction.h"
#include "compiler/ast/registry/node_component_registry.h"
#include <cstddef>
#include <llvm/Support/Allocator.h>
#include <vector>

class BuilderTreeInstruction : public IBuilderTree
{
private: 
    NodeComponentRegistry* _nodeComponentRegistry;
    RegistryInstruction* _registryInstructions;
    llvm::BumpPtrAllocator& _arena;

public: 

    BuilderTreeInstruction(NodeComponentRegistry* nodeComponentRegistry, RegistryInstruction* registryInstructions, llvm::BumpPtrAllocator& arena);
    ~BuilderTreeInstruction() override;

    // Delete copy and move constructors and assignment operators
    BuilderTreeInstruction(const BuilderTreeInstruction&) = delete;
    auto operator=(const BuilderTreeInstruction&) -> BuilderTreeInstruction& = delete;
    BuilderTreeInstruction(BuilderTreeInstruction&&) = delete;
    auto operator=(BuilderTreeInstruction&&) -> BuilderTreeInstruction& = delete;

    auto build(std::vector<Token>& tokens) -> INode* override;  
    auto build(std::vector<Token>& tokens, std::size_t index) -> INode* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;
};

#endif /* BDC39C44_6952_4793_8198_C083B106A089 */
