#ifndef D8FAA486_F5BA_43FB_BFFC_AB9990B46458
#define D8FAA486_F5BA_43FB_BFFC_AB9990B46458

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/RegistreStrategieEquation.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/Registre/RegistreFichier.h"
#include <memory>

class FacadeConfigurationEnvironnement
{
private:
    RegistreFonctionGlobale* _registreFonctionGlobale;
    RegistreFichier* _registreFichier;

    llvm::BumpPtrAllocator _arena;

    std::unique_ptr<LlvmBackend> _backend;
    std::unique_ptr<RegistreInstruction> _registreInstruction;
    std::unique_ptr<RegistreVariable> _registreVariable;
    std::unique_ptr<RegistreFonctionLocale> _registreFonctionLocale;
    std::unique_ptr<RegistreType> _registreType;
    std::unique_ptr<RetourContexteCompilation> _returnContextCompilation;
    std::unique_ptr<RegistreArgument> _registreArgument;
    std::unique_ptr<ContextGenCode> _context;

    RegistreStrategieEquation* _registreStrategieEquation;
    ConstructeurArbreInstruction* _constructeurArbreInstruction;
    ConstructeurEquationFlottante* _constructeurEquation;
    ParseurType* _parseurType;
    ContextParseur* _contextParseur;

    void creerRegistres();
    void creerContexte(const std::string& cheminFichier);
    void creerContextParseur();
    void enregistrerFonctionsExternes();
    void enregistrerTypesDeBase();
    void enregistrerStrategiesEquation();
    void enregistrerInstructions();

public:
    explicit FacadeConfigurationEnvironnement(RegistreFonctionGlobale* registreFonctionGlobale, RegistreFichier* registreFichier);
    ~FacadeConfigurationEnvironnement() = default;

    /// Initialise tout l'environnement de compilation en une seule étape
    void initialiser(const std::string& cheminFichier);
    
    ContextGenCode* getContext() const;
    llvm::BumpPtrAllocator& getArena();
    ConstructeurArbreInstruction* getConstructeurArbreInstruction() const;
    ConstructeurEquationFlottante* getConstructeurEquation() const;
};

#endif /* D8FAA486_F5BA_43FB_BFFC_AB9990B46458 */
