#ifndef C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F
#define C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F

#include <memory>
#include <iostream>
#include <stdexcept>
#include "RegistreInstruction.h"
#include "Pile/RegistreVariable.h"
#include "RegistreFonction.h"
#include "RegistreType.h"
#include "RegistreArgument.h"
#include "Pile/RetourContexteCompilation.h"
#include "../../LLVM/LlvmBackend.h"

struct ContextGenCode
{
    llvm::Value* valeurTemporaire;
    LlvmBackend* backend;
    RegistreInstruction* registreInstruction;
    RegistreVariable* registreVariable;
    RegistreFonction* registreFonction;
    RegistreType* registreType;
    RetourContexteCompilation* returnContextCompilation;
    RegistreArgument* registreArgument;

    ContextGenCode(
        LlvmBackend* backend,
        RegistreInstruction* registreInstruction,
        RegistreVariable* registreVariable,
        RegistreFonction* registreFonction,
        RegistreType* registreType,
        RetourContexteCompilation* returnContextCompilation,
        RegistreArgument* registreArgument,
        llvm::Value* valeurTemporaire
    ) 
    {
        try {
            if (backend == nullptr) {
                throw std::invalid_argument("Le backend LLVM ne peut pas être null");
            }
            if (registreInstruction == nullptr) {
                throw std::invalid_argument("Le registre d'instruction ne peut pas être null");
            }
            if (registreVariable == nullptr) {
                throw std::invalid_argument("Le registre de variable ne peut pas être null");
            }
            if (registreFonction == nullptr) {
                throw std::invalid_argument("Le registre de fonction ne peut pas être null");
            }
            if (registreType == nullptr) {
                throw std::invalid_argument("Le registre de type ne peut pas être null");
            }
            if (returnContextCompilation == nullptr) {
                throw std::invalid_argument("Le contexte de retour de compilation ne peut pas être null");
            }
            if (registreArgument == nullptr) {
                throw std::invalid_argument("Le registre d'argument ne peut pas être null");
            }

            this->valeurTemporaire = valeurTemporaire;
            this->backend = backend;
            this->registreInstruction = registreInstruction;
            this->registreVariable = registreVariable;
            this->registreFonction = registreFonction;
            this->registreType = registreType;
            this->returnContextCompilation = returnContextCompilation;
            this->registreArgument = registreArgument;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Erreur lors de l'initialisation du contexte de génération de code : " 
                      << e.what() << std::endl;
            throw;
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur inattendue lors de l'initialisation du contexte : " 
                      << e.what() << std::endl;
            throw;
        }
    }
};

#endif /* C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F */
