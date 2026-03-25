#ifndef C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F
#define C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "Compiler/Registry/RegistryFile.h"
#include "RegistryInstruction.h"
#include "Stack/RegistryVariable.h"
#include "RegistryFunction.h"
#include "RegistryType.h"
#include "RegistryArgument.h"
#include "RegistryClass.h"
#include "Stack/ReturnContextCompilation.h"
#include "../../LLVM/LlvmBackend.h"
#include "llvm/Support/Allocator.h"

struct ContextGenCode
{
private:
    RegistryFile* registryFile;
    Symbole valeurTemporaire;
    LlvmBackend* backend;
    RegistryInstruction* registryInstruction;
    RegistryVariable* registryVariable;
    RegistryFunctionGlobale* registryFunctionGlobale;
    RegistryFunctionLocale* registryFunctionLocale;
    RegistryType* registryType;
    ReturnContextCompilation* returnContextCompilation;
    RegistryArgument* registryArgument;
    RegistryClass* registryClass;
    llvm::BumpPtrAllocator* arena;
    std::string cheminFileCourant;
    std::string nomClasseCourante;

public:
    ContextGenCode(
        RegistryFile* p_registryFile,
        LlvmBackend* p_backend,
        RegistryInstruction* p_registryInstruction,
        RegistryVariable* p_registryVariable,
        RegistryFunctionGlobale* p_registryFunctionGlobale,
        RegistryFunctionLocale* p_registryFunctionLocale,
        RegistryType* p_registryType,
        ReturnContextCompilation* p_returnContextCompilation,
        RegistryArgument* p_registryArgument,
        RegistryClass* p_registryClass,
        Symbole p_valeurTemporaire,
        llvm::BumpPtrAllocator* p_arena,
        std::string p_cheminFileCourant
    ) 
    {
        try {
            if (p_cheminFileCourant.empty()) {
                throw std::invalid_argument("Le chemin du fichier courant ne peut pas être vide");
            }
            if (p_registryFile == nullptr)
            {
                throw std::invalid_argument("Le registry de fichier ne peux pas être null");
            }
            if (p_backend == nullptr) {
                throw std::invalid_argument("Le backend LLVM ne peut pas être null");
            }
            if (p_registryInstruction == nullptr) {
                throw std::invalid_argument("Le registry d'instruction ne peut pas être null");
            }
            if (p_registryVariable == nullptr) {
                throw std::invalid_argument("Le registry de variable ne peut pas être null");
            }
            if (p_registryFunctionGlobale == nullptr) {
                throw std::invalid_argument("Le registry de function globale ne peut pas être null");
            }
            if (p_registryFunctionLocale == nullptr) {
                throw std::invalid_argument("Le registry de function locale ne peut pas être null");
            }
            if (p_registryType == nullptr) {
                throw std::invalid_argument("Le registry de type ne peut pas être null");
            }
            if (p_returnContextCompilation == nullptr) {
                throw std::invalid_argument("Le contexte de return de compilation ne peut pas être null");
            }
            if (p_registryArgument == nullptr) {
                throw std::invalid_argument("Le registry d'argument ne peut pas être null");
            }
            if (p_arena == nullptr) {
                throw std::invalid_argument("L'arène ne peut pas être null");
            }
            if (p_registryClass == nullptr) {
                throw std::invalid_argument("Le registry de classe ne peut pas être null");
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error lors de la création du contexte de génération de code : " << e.what() << std::endl;
            throw;
        }
        this->registryFile = p_registryFile;
        this->cheminFileCourant = std::move(p_cheminFileCourant);
        this->backend = p_backend;
        this->registryInstruction = p_registryInstruction;
        this->registryVariable = p_registryVariable;
        this->registryFunctionGlobale = p_registryFunctionGlobale;
        this->registryFunctionLocale = p_registryFunctionLocale;
        this->registryType = p_registryType;
        this->returnContextCompilation = p_returnContextCompilation;
        this->registryArgument = p_registryArgument;
        this->registryClass = p_registryClass;
        this->valeurTemporaire = p_valeurTemporaire;
        this->arena = p_arena;
    }

    
    void modifierValeurTemporaire(Symbole p_valeurTemporaire) { valeurTemporaire = p_valeurTemporaire; }
    void modifierNomClasseCourante(std::string p_nomClasseCourante) { nomClasseCourante = std::move(p_nomClasseCourante); }

    // Getters
    [[nodiscard]] auto getRegistryFile() const -> RegistryFile* { return registryFile; }
    [[nodiscard]] auto getValeurTemporaire() const -> Symbole { return valeurTemporaire; }
    [[nodiscard]] auto getBackend() const -> LlvmBackend* { return backend; }
    [[nodiscard]] auto getRegistryInstruction() const -> RegistryInstruction* { return registryInstruction; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getRegistryFunctionGlobale() const -> RegistryFunctionGlobale* { return registryFunctionGlobale; }
    [[nodiscard]] auto getRegistryFunctionLocale() const -> RegistryFunctionLocale* { return registryFunctionLocale; }
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return registryType; }
    [[nodiscard]] auto getReturnContextCompilation() const -> ReturnContextCompilation* { return returnContextCompilation; }
    [[nodiscard]] auto getRegistryArgument() const -> RegistryArgument* { return registryArgument; }
    [[nodiscard]] auto getRegistryClass() const -> RegistryClass* { return registryClass; }
    [[nodiscard]] auto getArena() const -> llvm::BumpPtrAllocator* { return arena; }
    [[nodiscard]] auto getCheminFileCourant() const -> const std::string& { return cheminFileCourant; }
    [[nodiscard]] auto getNomClasseCourante() const -> const std::string& { return nomClasseCourante; }
};

#endif /* C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F */
