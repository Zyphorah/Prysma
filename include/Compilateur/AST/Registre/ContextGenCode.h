#ifndef C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F
#define C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F

#include <memory>
#include "RegistreInstruction.h"
#include "Pile/RegistreVariable.h"
#include "RegistreFonction.h"
#include "RegistreType.h"
#include "RegistreArgument.h"
#include "Pile/ReturnContextCompilation.h"
#include "../../LLVM/LLVMBackend.h"

struct ContextGenCode
{
    llvm::Value* valeurTemporaire;
    std::shared_ptr<LLVMBackend> backend;
    std::shared_ptr<RegistreInstruction> registreInstruction;
    std::shared_ptr<RegistreVariable> registreVariable;
    std::shared_ptr<RegistreFonction> registreFonction;
    std::shared_ptr<RegistreType> registreType;
    std::shared_ptr<ReturnContextCompilation> returnContextCompilation;
    std::shared_ptr<RegistreArgument> registreArgument;

    ContextGenCode(
        std::shared_ptr<LLVMBackend> backend,
        std::shared_ptr<RegistreInstruction> registreInstruction,
        std::shared_ptr<RegistreVariable> registreVariable,
        std::shared_ptr<RegistreFonction> registreFonction,
        std::shared_ptr<RegistreType> registreType,
        std::shared_ptr<ReturnContextCompilation> returnContextCompilation,
        std::shared_ptr<RegistreArgument> registreArgument,
        llvm::Value* valeurTemporaire
    ) 
        : valeurTemporaire(valeurTemporaire),
          backend(std::move(backend)),
          registreInstruction(std::move(registreInstruction)),
          registreVariable(std::move(registreVariable)),
          registreFonction(std::move(registreFonction)),
          registreType(std::move(registreType)),
          returnContextCompilation(std::move(returnContextCompilation)),
          registreArgument(std::move(registreArgument))
          
    {
    }
};

#endif /* C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F */
