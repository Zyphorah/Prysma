#ifndef F0244D90_670B_498F_BCBB_0FCEAB580007
#define F0244D90_670B_498F_BCBB_0FCEAB580007

#include <memory>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include "LLVM/LLVMSerializer.h"

class LLVMBackend
{
    private:
           // ===== Initialisation LLVM =====
        llvm::LLVMContext _context;
        std::unique_ptr<llvm::Module> _module;
        std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> _builder;

    public :
        LLVMBackend()
            : _context(),
              _module(std::make_unique<llvm::Module>("PrysmaModule", _context)),
              _builder(std::make_unique<llvm::IRBuilder<llvm::NoFolder>>(_context))
        {
        }
        ~LLVMBackend() = default;
        void creationFonctionLLVM();
};

#endif /* F0244D90_670B_498F_BCBB_0FCEAB580007 */
 
