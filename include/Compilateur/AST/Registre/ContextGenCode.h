#ifndef C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F
#define C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F

#include <iostream>
#include <stdexcept>
#include "Compilateur/Registre/RegistreFichier.h"
#include "RegistreInstruction.h"
#include "Pile/RegistreVariable.h"
#include "RegistreFonction.h"
#include "RegistreType.h"
#include "RegistreArgument.h"
#include "RegistreClass.h"
#include "Pile/RetourContexteCompilation.h"
#include "../../LLVM/LlvmBackend.h"
#include "llvm/Support/Allocator.h"

struct ContextGenCode
{
    RegistreFichier* registreFichier;
    Symbole valeurTemporaire;
    LlvmBackend* backend;
    RegistreInstruction* registreInstruction;
    RegistreVariable* registreVariable;
    RegistreFonctionGlobale* registreFonctionGlobale;
    RegistreFonctionLocale* registreFonctionLocale;
    RegistreType* registreType;
    RetourContexteCompilation* returnContextCompilation;
    RegistreArgument* registreArgument;
    RegistreClass* registreClass;
    llvm::BumpPtrAllocator* arena;
    std::string cheminFichierCourant;
    std::string nomClasseCourante = "";

    ContextGenCode(
        RegistreFichier* p_registreFichier,
        LlvmBackend* p_backend,
        RegistreInstruction* p_registreInstruction,
        RegistreVariable* p_registreVariable,
        RegistreFonctionGlobale* p_registreFonctionGlobale,
        RegistreFonctionLocale* p_registreFonctionLocale,
        RegistreType* p_registreType,
        RetourContexteCompilation* p_returnContextCompilation,
        RegistreArgument* p_registreArgument,
        RegistreClass* p_registreClass,
        Symbole p_valeurTemporaire,
        llvm::BumpPtrAllocator* p_arena,
        std::string p_cheminFichierCourant
    ) 
    {
        try {
            if (p_cheminFichierCourant.empty()) {
                throw std::invalid_argument("Le chemin du fichier courant ne peut pas être vide");
            }
            if (p_registreFichier == nullptr)
            {
                throw std::invalid_argument("Le registre de fichier ne peux pas être null");
            }
            if (p_backend == nullptr) {
                throw std::invalid_argument("Le backend LLVM ne peut pas être null");
            }
            if (p_registreInstruction == nullptr) {
                throw std::invalid_argument("Le registre d'instruction ne peut pas être null");
            }
            if (p_registreVariable == nullptr) {
                throw std::invalid_argument("Le registre de variable ne peut pas être null");
            }
            if (p_registreFonctionGlobale == nullptr) {
                throw std::invalid_argument("Le registre de fonction globale ne peut pas être null");
            }
            if (p_registreFonctionLocale == nullptr) {
                throw std::invalid_argument("Le registre de fonction locale ne peut pas être null");
            }
            if (p_registreType == nullptr) {
                throw std::invalid_argument("Le registre de type ne peut pas être null");
            }
            if (p_returnContextCompilation == nullptr) {
                throw std::invalid_argument("Le contexte de retour de compilation ne peut pas être null");
            }
            if (p_registreArgument == nullptr) {
                throw std::invalid_argument("Le registre d'argument ne peut pas être null");
            }
            if (p_arena == nullptr) {
                throw std::invalid_argument("L'arène ne peut pas être null");
            }
            if (p_registreClass == nullptr) {
                throw std::invalid_argument("Le registre de classe ne peut pas être null");
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Erreur lors de la création du contexte de génération de code : " << e.what() << std::endl;
            throw;
        }
        this->cheminFichierCourant = std::move(p_cheminFichierCourant);
        this->backend = p_backend;
        this->registreInstruction = p_registreInstruction;
        this->registreVariable = p_registreVariable;
        this->registreFonctionGlobale = p_registreFonctionGlobale;
        this->registreFonctionLocale = p_registreFonctionLocale;
        this->registreType = p_registreType;
        this->returnContextCompilation = p_returnContextCompilation;
        this->registreArgument = p_registreArgument;
        this->registreClass = p_registreClass;
        this->valeurTemporaire = p_valeurTemporaire;
        this->arena = p_arena;
    }
};

#endif /* C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F */
