#include "Compiler/AST/BuilderTreeInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Nodes/NodeInstruction.h"
#include "Compiler/AST/Registry/RegistryInstruction.h"
#include "Compiler/Parser/Interfaces/IParser.h"
#include "Compiler/ManagerError.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <llvm/Support/Allocator.h>
#include <vector>

BuilderTreeInstruction::BuilderTreeInstruction(RegistryInstruction* registryInstructions, llvm::BumpPtrAllocator& arena)
    : _registryInstructions(registryInstructions), _arena(arena)
{
    
}

BuilderTreeInstruction::~BuilderTreeInstruction()
= default;

auto BuilderTreeInstruction::build(std::vector<Token>& tokens, int& index) -> INode*
{
    if (!_registryInstructions->exists(tokens[static_cast<size_t>(index)].type)) {
        throw CompilationError("Unknown instruction: '" + tokens[static_cast<size_t>(index)].value + "'", Line(tokens[static_cast<size_t>(index)].line), Column(tokens[static_cast<size_t>(index)].column));
    }
    IParser* parentNode = _registryInstructions->get(tokens[static_cast<size_t>(index)].type);
    INode* child = parentNode->parse(tokens, index);
   
    return child;
}


auto BuilderTreeInstruction::build(std::vector<Token>& tokens) -> INode*
{
    int index = 0; 
    // Create a global container for the whole file
    auto* globalProgram = allocate<NodeInstruction>();
    
    while (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_EOF) {
        INode* child = build(tokens, index);
        if (child != nullptr) {
            globalProgram->addInstruction(child);
        }
    }
    return globalProgram;
}

auto BuilderTreeInstruction::getArena() -> llvm::BumpPtrAllocator&
{
    return _arena;
}