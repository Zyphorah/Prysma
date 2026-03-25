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

auto BuilderTreeInstruction::construire(std::vector<Token>& tokens, int& index) -> INode*
{
    if (!_registryInstructions->existe(tokens[static_cast<size_t>(index)].type)) {
        throw ErrorCompilation("Instruction inconnue : '" + tokens[static_cast<size_t>(index)].value + "'", Ligne(tokens[static_cast<size_t>(index)].ligne), Colonne(tokens[static_cast<size_t>(index)].colonne));
    }
    IParser* ParentNode = _registryInstructions->recuperer(tokens[static_cast<size_t>(index)].type);
    INode* child = ParentNode->parser(tokens, index);
   
    return child;
}


auto BuilderTreeInstruction::construire(std::vector<Token>& tokens) -> INode*
{
    int index = 0; 
    // On crée un conteneur global pour tout le fichier
    auto* programmeGlobal = allouer<NodeInstruction>();
    
    while (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_EOF) {
        INode* child = construire(tokens, index);
        if (child != nullptr) {
            programmeGlobal->ajouterInstruction(child);
        }
    }
    return programmeGlobal;
}

auto BuilderTreeInstruction::getArena() -> llvm::BumpPtrAllocator&
{
    return _arena;
}