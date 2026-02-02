#ifndef NOEUD_MAIN_H
#define NOEUD_MAIN_H

#include "Compilateur/AST/Noeuds/Instruction.h"
#include <llvm/IR/Value.h>

class NoeudMain : public Instruction
{
public: 

    NoeudMain();
    ~NoeudMain();

    llvm::Value* genCode() override;
};

#endif /* NOEUD_MAIN_H */
