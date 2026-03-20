#ifndef BDC39C44_6952_4793_8198_C083B106A089
#define BDC39C44_6952_4793_8198_C083B106A089

#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include <llvm/Support/Allocator.h>
#include <vector>

class ConstructeurArbreInstruction : public IConstructeurArbre
{
private: 
    RegistreInstruction* _registreInstructions;
    llvm::BumpPtrAllocator& _arena;
public: 

    ConstructeurArbreInstruction(RegistreInstruction* registreInstructions, llvm::BumpPtrAllocator& arena);
    ~ConstructeurArbreInstruction() override;

    // Delete copy and move constructors and assignment operators
    ConstructeurArbreInstruction(const ConstructeurArbreInstruction&) = delete;
    auto operator=(const ConstructeurArbreInstruction&) -> ConstructeurArbreInstruction& = delete;
    ConstructeurArbreInstruction(ConstructeurArbreInstruction&&) = delete;
    auto operator=(ConstructeurArbreInstruction&&) -> ConstructeurArbreInstruction& = delete;

    auto construire(std::vector<Token>& tokens) -> INoeud* override;  
    auto construire(std::vector<Token>& tokens, int& index) -> INoeud* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;

};

#endif /* BDC39C44_6952_4793_8198_C083B106A089 */
