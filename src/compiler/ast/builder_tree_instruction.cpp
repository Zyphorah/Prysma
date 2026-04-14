#include "compiler/ast/builder_tree_instruction.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/registry_instruction.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/parser/interfaces/i_parser.h"
#include "compiler/manager_error.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include "llvm/ADT/SmallVector.h"
#include <cstddef>
#include <llvm-18/llvm/ADT/ArrayRef.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/FormatVariadic.h>
#include <utility>
#include <vector>

BuilderTreeInstruction::BuilderTreeInstruction(NodeComponentRegistry* nodeComponentRegistry, RegistryInstruction* registryInstructions, llvm::BumpPtrAllocator& arena)
    : _nodeComponentRegistry(nodeComponentRegistry), _registryInstructions(registryInstructions), _arena(arena) {}

BuilderTreeInstruction::~BuilderTreeInstruction() = default;

auto BuilderTreeInstruction::build(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    if (!_registryInstructions->exists(tokens[index].type)) {
        throw CompilationError(llvm::formatv(
            "Unknown instruction: '{0}'", tokens[index].value).str(),
                Line(tokens[index].line),
                Column(tokens[index].column)
            );
    }
    IParser* parentNode = _registryInstructions->get(tokens[index].type);
    INode* child = parentNode->parse(tokens, index);
                                                                                                                                                                                                                                                                                                         
    return child;
}


auto BuilderTreeInstruction::build(std::vector<Token>& tokens) -> INode*
{
    std::size_t index = 0; 
    llvm::SmallVector<INode*, 64> children;
    
    while (index < tokens.size() && tokens[index].type != TOKEN_EOF) {
        INode* child = build(tokens, index);
        if (child != nullptr) {
            children.push_back(child);
        }
    }

    auto* new_node = allocate<NodeInstruction>(_nodeComponentRegistry->getNextId()); // un nouveau noeud d'instruction (INode*) est allocated dans le pool
    llvm::ArrayRef<INode*> arr_ref = allocateArray<INode*>(children); // les noeuds enfants (INode*) sont allocated dans le pool 

    _nodeComponentRegistry->emplace<NodeInstructionComponents>(new_node->getNodeId(), arr_ref); // le llvm::ArrayRef est inséré dans le registre au ID du nouveau noeud

    return new_node;
}

auto BuilderTreeInstruction::getArena() -> llvm::BumpPtrAllocator&
{
    return _arena;
}