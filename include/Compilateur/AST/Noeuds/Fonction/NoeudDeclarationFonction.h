#ifndef A2837407_B466_49AE_8A29_4BFC0A5D0461
#define A2837407_B466_49AE_8A29_4BFC0A5D0461

#include "Compilateur/AST/Noeuds/Instruction.h"
class NoeudDeclarationFonction : public Instruction
{
private:
    

public:
  NoeudDeclarationFonction();
  ~NoeudDeclarationFonction();

  llvm::Value* genCode() override;

};

#endif /* A2837407_B466_49AE_8A29_4BFC0A5D0461 */
