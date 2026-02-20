#ifndef B6CEBD3F_F0AA_40D7_B687_5859371B60F9
#define B6CEBD3F_F0AA_40D7_B687_5859371B60F9

#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"
#include "Compilateur/AnalyseSyntaxique/ParseurBase.h"
#include <llvm-18/llvm/Support/Allocator.h>

class StrategieString : public IStrategieEquation, public ParseurBase
{
private: 
    llvm::BumpPtrAllocator& _allocator;
public:

    StrategieString(llvm::BumpPtrAllocator& allocator);
    ~StrategieString();

    INoeud* construire(std::vector<Token>& equation) override;
};

#endif /* B6CEBD3F_F0AA_40D7_B687_5859371B60F9 */
