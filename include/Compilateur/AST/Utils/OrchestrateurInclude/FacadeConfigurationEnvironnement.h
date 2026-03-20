#ifndef D8FAA486_F5BA_43FB_BFFC_AB9990B46458
#define D8FAA486_F5BA_43FB_BFFC_AB9990B46458

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreExpression.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/Registre/RegistreFichier.h"
#include <llvm/Support/Allocator.h>
#include <memory>
#include <string>

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
    std::unique_ptr<RegistreClass> _registreClass;
    std::unique_ptr<ContextGenCode> _context;

    RegistreExpression* _registreExpression;
    ConstructeurArbreInstruction* _constructeurArbreInstruction;
    ConstructeurEquationFlottante* _constructeurEquation;
    ParseurType* _parseurType;
    ContextParseur* _contextParseur;
    ContexteExpression* _contexteExpression;

    void creerRegistres();
    void creerContexte(const std::string& cheminFichier);
    void creerContextParseur();
    void enregistrerFonctionsExternes();
    void enregistrerTypesDeBase();
    void enregistrerExpressions();
    void enregistrerInstructions();

public:
    explicit FacadeConfigurationEnvironnement(RegistreFonctionGlobale* registreFonctionGlobale, RegistreFichier* registreFichier);
    ~FacadeConfigurationEnvironnement();

    FacadeConfigurationEnvironnement(const FacadeConfigurationEnvironnement&) = delete;
    auto operator=(const FacadeConfigurationEnvironnement&) -> FacadeConfigurationEnvironnement& = delete;
    FacadeConfigurationEnvironnement(FacadeConfigurationEnvironnement&&) = delete;
    auto operator=(FacadeConfigurationEnvironnement&&) -> FacadeConfigurationEnvironnement& = delete;

    /// Initialise tout l'environnement de compilation en une seule étape
    void initialiser(const std::string& cheminFichier);
    
    [[nodiscard]] auto getContext() const -> ContextGenCode*;
    llvm::BumpPtrAllocator& getArena();
    [[nodiscard]] auto getConstructeurArbreInstruction() const -> ConstructeurArbreInstruction*;
    [[nodiscard]] auto getConstructeurEquation() const -> ConstructeurEquationFlottante*;
};

#endif /* D8FAA486_F5BA_43FB_BFFC_AB9990B46458 */
