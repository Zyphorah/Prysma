#ifndef BDC39C44_6952_4793_8198_C083B106A089
#define BDC39C44_6952_4793_8198_C083B106A089

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/AST/Registry/RegistryInstruction.h"
#include <llvm/Support/Allocator.h>
#include <vector>

class BuilderTreeInstruction : public IBuilderTree
{
private: 
    RegistryInstruction* _registryInstructions;
    llvm::BumpPtrAllocator& _arena;
public: 

    BuilderTreeInstruction(RegistryInstruction* registryInstructions, llvm::BumpPtrAllocator& arena);
    ~BuilderTreeInstruction() override;

    // Delete copy and move constructors and assignment operators
    BuilderTreeInstruction(const BuilderTreeInstruction&) = delete;
    auto operator=(const BuilderTreeInstruction&) -> BuilderTreeInstruction& = delete;
    BuilderTreeInstruction(BuilderTreeInstruction&&) = delete;
    auto operator=(BuilderTreeInstruction&&) -> BuilderTreeInstruction& = delete;

    auto construire(std::vector<Token>& tokens) -> INode* override;  
    auto construire(std::vector<Token>& tokens, int& index) -> INode* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;

};

#endif /* BDC39C44_6952_4793_8198_C083B106A089 */
