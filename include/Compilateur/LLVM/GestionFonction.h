#ifndef D2944365_C1DD_41F8_A211_BFF33402A958
#define D2944365_C1DD_41F8_A211_BFF33402A958

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <vector>
#include <string>

class VisiteurGeneralGenCode;
class NoeudArgFonction;
class NoeudDeclarationFonction;
class NoeudAppelFonction;

struct ArgumentsCodeGen {
    std::vector<llvm::Type*> argTypes;
    std::vector<NoeudArgFonction*> arguments;
};

// Génération de la Déclaration

class GenerateurDeclarationFonction {
private:
    ContextGenCode* _contextGenCode;
    NoeudDeclarationFonction* _noeudDeclarationFonction;
    IVisiteur* _visiteurGeneralCodeGen;

    virtual auto creerFonction() -> llvm::Function* = 0;
    virtual void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) = 0;

public:
    GenerateurDeclarationFonction(ContextGenCode* context, NoeudDeclarationFonction* noeud, IVisiteur* visiteur);
    virtual ~GenerateurDeclarationFonction() = default;

    [[nodiscard]] auto getContextGenCode() const -> ContextGenCode* { return _contextGenCode; }
    [[nodiscard]] auto getNoeudDeclarationFonction() const -> NoeudDeclarationFonction* { return _noeudDeclarationFonction; }
    [[nodiscard]] auto getVisiteurGeneralCodeGen() const -> IVisiteur* { return _visiteurGeneralCodeGen; }

    GenerateurDeclarationFonction(const GenerateurDeclarationFonction&) = delete;
    auto operator=(const GenerateurDeclarationFonction&) -> GenerateurDeclarationFonction& = delete;
    GenerateurDeclarationFonction(GenerateurDeclarationFonction&&) = delete;
    auto operator=(GenerateurDeclarationFonction&&) -> GenerateurDeclarationFonction& = delete;

    void declarerFonction();
    
    static auto creer(ContextGenCode* context, NoeudDeclarationFonction* noeud, IVisiteur* visiteur) -> std::unique_ptr<GenerateurDeclarationFonction>;
};

class GenerateurDeclarationStandard : public GenerateurDeclarationFonction {
private:
    auto creerFonction() -> llvm::Function* override;
    void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using GenerateurDeclarationFonction::GenerateurDeclarationFonction;
};

class GenerateurDeclarationMethode : public GenerateurDeclarationFonction {
private:
    auto creerFonction() -> llvm::Function* override;
    void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using GenerateurDeclarationFonction::GenerateurDeclarationFonction;
};


// Génération de l'Appel
class GenerateurAppelFonction {
private:
    ContextGenCode* _contextGenCode;
    IVisiteur* _visiteurGeneralCodeGen;

    virtual auto obtenirFonctionLocale(const std::string& nomFonction) -> const SymboleFonctionLocale* = 0;

public:
    GenerateurAppelFonction(ContextGenCode* context, IVisiteur* visiteur);
    virtual ~GenerateurAppelFonction() = default;

    [[nodiscard]] auto getContextGenCode() const -> ContextGenCode* { return _contextGenCode; }
    [[nodiscard]] auto getVisiteurGeneralCodeGen() const -> IVisiteur* { return _visiteurGeneralCodeGen; }

    GenerateurAppelFonction(const GenerateurAppelFonction&) = delete;
    auto operator=(const GenerateurAppelFonction&) -> GenerateurAppelFonction& = delete;
    GenerateurAppelFonction(GenerateurAppelFonction&&) = delete;
    auto operator=(GenerateurAppelFonction&&) -> GenerateurAppelFonction& = delete;

    void genererAppelFonction(NoeudAppelFonction* noeud);
    static auto creer(ContextGenCode* context, IVisiteur* visiteur) -> std::unique_ptr<GenerateurAppelFonction>;
};

class GenerateurAppelStandard : public GenerateurAppelFonction {
private:
    const SymboleFonctionLocale* obtenirFonctionLocale(const std::string& nomFonction) override;
public:
    using GenerateurAppelFonction::GenerateurAppelFonction;
};

class GenerateurAppelMethode : public GenerateurAppelFonction {
private:
    const SymboleFonctionLocale* obtenirFonctionLocale(const std::string& nomFonction) override;
public:
    using GenerateurAppelFonction::GenerateurAppelFonction;
};

// Gestion des fonctions Natives (Built-ins)

class RegistreBuiltIns {
public:
    static auto estBuiltIn(const std::string& nom) -> bool;
    static void genererAppel(const std::string& nom, NoeudAppelFonction* noeud, ContextGenCode* context, IVisiteur* visiteur);
};

#endif /* D2944365_C1DD_41F8_A211_BFF33402A958 */
