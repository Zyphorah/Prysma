#ifndef D2944365_C1DD_41F8_A211_BFF33402A958
#define D2944365_C1DD_41F8_A211_BFF33402A958

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
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
protected:
    ContextGenCode* _contextGenCode;
    NoeudDeclarationFonction* _noeudDeclarationFonction;
    IVisiteur* _visiteurGeneralCodeGen;

    virtual llvm::Function* creerFonction() = 0;
    virtual void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) = 0;

public:
    GenerateurDeclarationFonction(ContextGenCode* context, NoeudDeclarationFonction* noeud, IVisiteur* visiteur);
    virtual ~GenerateurDeclarationFonction() = default;

    void declarerFonction();
    
    static std::unique_ptr<GenerateurDeclarationFonction> creer(ContextGenCode* context, NoeudDeclarationFonction* noeud, IVisiteur* visiteur);
};

class GenerateurDeclarationStandard : public GenerateurDeclarationFonction {
protected:
    llvm::Function* creerFonction() override;
    void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using GenerateurDeclarationFonction::GenerateurDeclarationFonction;
};

class GenerateurDeclarationMethode : public GenerateurDeclarationFonction {
protected:
    llvm::Function* creerFonction() override;
    void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using GenerateurDeclarationFonction::GenerateurDeclarationFonction;
};


// Génération de l'Appel

class GenerateurAppelFonction {
protected:
    ContextGenCode* _contextGenCode;
    IVisiteur* _visiteurGeneralCodeGen;

    virtual const SymboleFonctionLocale* obtenirFonctionLocale(const std::string& nomFonction) = 0;

public:
    GenerateurAppelFonction(ContextGenCode* context, IVisiteur* visiteur);
    virtual ~GenerateurAppelFonction() = default;

    void genererAppelFonction(NoeudAppelFonction* noeud);
    static std::unique_ptr<GenerateurAppelFonction> creer(ContextGenCode* context, IVisiteur* visiteur);
};

class GenerateurAppelStandard : public GenerateurAppelFonction {
protected:
    const SymboleFonctionLocale* obtenirFonctionLocale(const std::string& nomFonction) override;
public:
    using GenerateurAppelFonction::GenerateurAppelFonction;
};

class GenerateurAppelMethode : public GenerateurAppelFonction {
protected:
    const SymboleFonctionLocale* obtenirFonctionLocale(const std::string& nomFonction) override;
public:
    using GenerateurAppelFonction::GenerateurAppelFonction;
};

// Gestion des fonctions Natives (Built-ins)

class RegistreBuiltIns {
public:
    static bool estBuiltIn(const std::string& nom);
    static void genererAppel(const std::string& nom, NoeudAppelFonction* noeud, ContextGenCode* context, IVisiteur* visiteur);
};

#endif /* D2944365_C1DD_41F8_A211_BFF33402A958 */
