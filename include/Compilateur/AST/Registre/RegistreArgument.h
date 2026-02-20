#ifndef CCD2D336_FEA8_4097_8361_E253E2A1166E
#define CCD2D336_FEA8_4097_8361_E253E2A1166E
#include <llvm-18/llvm/IR/Value.h>
#include <memory>
#include <vector>

class RegistreArgument
{
private: 
    std::vector<llvm::Value*> _vecArg;

public: 
   RegistreArgument();
   void ajouter(llvm::Value* arg);
   std::vector<llvm::Value*>& recuperer();
   void vider();
};

#endif /* CCD2D336_FEA8_4097_8361_E253E2A1166E */
