#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/GestionFonction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include "Compilateur/Visiteur/Extracteurs/ExtracteurArgFonction.h"
#include <cstddef>
#include <cstdint>
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>


// Génération de la Déclaration

std::unique_ptr<GenerateurDeclarationFonction> GenerateurDeclarationFonction::creer(ContextGenCode* context, NoeudDeclarationFonction* noeud, IVisiteur* visiteur)
{
    if (!context->getNomClasseCourante().empty()) {
        return std::make_unique<GenerateurDeclarationMethode>(context, noeud, visiteur);
    } 
    return std::make_unique<GenerateurDeclarationStandard>(context, noeud, visiteur);
}

GenerateurDeclarationFonction::GenerateurDeclarationFonction(ContextGenCode* contextGenCode, NoeudDeclarationFonction* noeudDeclarationFonction, IVisiteur* visiteurGeneralCodeGen) 
:   _contextGenCode(contextGenCode),
    _noeudDeclarationFonction(noeudDeclarationFonction),
    _visiteurGeneralCodeGen(visiteurGeneralCodeGen)
{
}

llvm::Function* GenerateurDeclarationStandard::creerFonction()
{
    std::string nomFonction = getNoeudDeclarationFonction()->getNom();
    
    const auto& symbolePtr = getContextGenCode()->getRegistreFonctionLocale()->recuperer(nomFonction);
    const auto* symbole = static_cast<const SymboleFonctionLocale*>(symbolePtr.get()); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    
    llvm::Function* function = symbole->fonction;

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(getContextGenCode()->getBackend()->getContext(), "entry", function);
    getContextGenCode()->getBackend()->getBuilder().SetInsertPoint(entryBlock);

    return function;
}

llvm::Function* GenerateurDeclarationMethode::creerFonction()
{
    std::string nomFonction = getNoeudDeclarationFonction()->getNom();
    std::string nomClasse = getContextGenCode()->getNomClasseCourante();
    auto const& classInfo = getContextGenCode()->getRegistreClass()->recuperer(nomClasse);
    const auto& symbolePtr = classInfo->getRegistreFonctionLocale()->recuperer(nomFonction);
    const auto* symbole = static_cast<const SymboleFonctionLocale*>(symbolePtr.get()); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    
    llvm::Function* function = symbole->fonction;

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(getContextGenCode()->getBackend()->getContext(), "entry", function);
    getContextGenCode()->getBackend()->getBuilder().SetInsertPoint(entryBlock);

    return function;
}

void GenerateurDeclarationMethode::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args)
{
    size_t argIndex = 0;
    
    if (function->arg_size() > 0) {
        llvm::Argument* thisArg = function->getArg(0);
        thisArg->setName("this");
        
        llvm::Type* argType = thisArg->getType();
        llvm::AllocaInst* alloca = getContextGenCode()->getBackend()->getBuilder().CreateAlloca(argType);
        getContextGenCode()->getBackend()->getBuilder().CreateStore(thisArg, alloca);
        
        Token thisToken;
        thisToken.value = "this";
        thisToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symboleThis;
        symboleThis = Symbole(alloca, symboleThis.getType(), symboleThis.getTypePointeElement());
        symboleThis = Symbole(symboleThis.getAdresse(), nullptr, symboleThis.getTypePointeElement()); 
        
        getContextGenCode()->getRegistreVariable()->enregistrer(thisToken, symboleThis);
        argIndex = 1;
    }

    for (auto* noeudArg : args.arguments) {
        llvm::Argument* arg = function->getArg(static_cast<unsigned int>(argIndex));
        arg->setName(noeudArg->getNom());
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = getContextGenCode()->getBackend()->getBuilder().CreateAlloca(argType);
        getContextGenCode()->getBackend()->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = noeudArg->getNom();
        argumentToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symbole;
        symbole = Symbole(alloca, symbole.getType(), symbole.getTypePointeElement());
        symbole = Symbole(symbole.getAdresse(), noeudArg->getType(), symbole.getTypePointeElement());
        getContextGenCode()->getRegistreVariable()->enregistrer(argumentToken, symbole);
        
        argIndex++;
    }
}

void GenerateurDeclarationStandard::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args)
{
    size_t argIndex = 0;

    for (auto* noeudArg : args.arguments) {
        llvm::Argument* arg = function->getArg(static_cast<unsigned int>(argIndex));
        arg->setName(noeudArg->getNom());
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = getContextGenCode()->getBackend()->getBuilder().CreateAlloca(argType);
        getContextGenCode()->getBackend()->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = noeudArg->getNom();
        argumentToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symbole;
        symbole = Symbole(alloca, symbole.getType(), symbole.getTypePointeElement());
        symbole = Symbole(symbole.getAdresse(), noeudArg->getType(), symbole.getTypePointeElement());
        getContextGenCode()->getRegistreVariable()->enregistrer(argumentToken, symbole);
        
        argIndex++;
    }
}

void GenerateurDeclarationFonction::declarerFonction()
{
    llvm::Type* typeDeRetour = getNoeudDeclarationFonction()->getTypeRetour()->genererTypeLLVM(getContextGenCode()->getBackend()->getContext());
    
    ArgumentsCodeGen argumentsCodeGen;
    if (getNoeudDeclarationFonction() != nullptr) {
        for (INoeud* noeud : getNoeudDeclarationFonction()->getArguments()) {
            ExtracteurArgFonction extracteur;
            noeud->accept(&extracteur);
            if (extracteur.getArg() != nullptr) {
                argumentsCodeGen.arguments.push_back(extracteur.getArg());
                llvm::Type* argType = extracteur.getArg()->getType()->genererTypeLLVM(getContextGenCode()->getBackend()->getContext());
                argumentsCodeGen.argTypes.push_back(argType);
            }
        }
    }

    llvm::Function* function = creerFonction();
    
    getContextGenCode()->getReturnContextCompilation()->piler(getNoeudDeclarationFonction()->getTypeRetour());
    getContextGenCode()->getRegistreVariable()->piler();
    
    traiterArgumentsConstruit(function, argumentsCodeGen);

    getNoeudDeclarationFonction()->getCorps()->accept(_visiteurGeneralCodeGen);

    llvm::BasicBlock* blocCourant = getContextGenCode()->getBackend()->getBuilder().GetInsertBlock();
    if (blocCourant != nullptr && typeDeRetour->isVoidTy())
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
        if (blocCourant->getTerminator() == nullptr)
        {
            getContextGenCode()->getBackend()->getBuilder().CreateRetVoid();
        }
#pragma GCC diagnostic pop
    }

    getContextGenCode()->getRegistreVariable()->depiler();
    getContextGenCode()->getReturnContextCompilation()->depiler();

    getContextGenCode()->modifierValeurTemporaire(Symbole(function, getContextGenCode()->getValeurTemporaire().getType(), getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
}

// Génération de l'Appel

std::unique_ptr<GenerateurAppelFonction> GenerateurAppelFonction::creer(ContextGenCode* context, IVisiteur* visiteur)
{
    if (!context->getNomClasseCourante().empty()) {
        return std::make_unique<GenerateurAppelMethode>(context, visiteur);
    }
    return std::make_unique<GenerateurAppelStandard>(context, visiteur);
}

GenerateurAppelFonction::GenerateurAppelFonction(ContextGenCode* context, IVisiteur* visiteur)
    : _contextGenCode(context), _visiteurGeneralCodeGen(visiteur)
{
}

const SymboleFonctionLocale* GenerateurAppelMethode::obtenirFonctionLocale(const std::string& nomFonction)
{
    std::string nomClasse = getContextGenCode()->getNomClasseCourante();
    auto const& classInfo = getContextGenCode()->getRegistreClass()->recuperer(nomClasse);
    if(classInfo->getRegistreFonctionLocale()->existe(nomFonction)){
        const auto& symbolePtr = classInfo->getRegistreFonctionLocale()->recuperer(nomFonction);
        return static_cast<const SymboleFonctionLocale*>(symbolePtr.get()); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    }
    
    if (getContextGenCode()->getRegistreFonctionLocale()->existe(nomFonction)) {
        const auto& symbolePtr = getContextGenCode()->getRegistreFonctionLocale()->recuperer(nomFonction);
        return static_cast<const SymboleFonctionLocale*>(symbolePtr.get()); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    }

    throw std::runtime_error("Méthode ou fonction introuvable dans la portée de la classe : " + nomFonction);
}

const SymboleFonctionLocale* GenerateurAppelStandard::obtenirFonctionLocale(const std::string& nomFonction)
{
    if (getContextGenCode()->getRegistreFonctionLocale()->existe(nomFonction)) {
        const auto& symbolePtr = getContextGenCode()->getRegistreFonctionLocale()->recuperer(nomFonction);
        return static_cast<const SymboleFonctionLocale*>(symbolePtr.get()); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    }

    throw std::runtime_error("Fonction introuvable dans le registre local : " + nomFonction);
}

void GenerateurAppelFonction::genererAppelFonction(NoeudAppelFonction* noeudAppelFonction)
{
    std::string nomFonction = noeudAppelFonction->getNomFonction().value;

    if (RegistreBuiltIns::estBuiltIn(nomFonction)) {
        RegistreBuiltIns::genererAppel(nomFonction, noeudAppelFonction, getContextGenCode(), _visiteurGeneralCodeGen);
        return; 
    }

    getContextGenCode()->getRegistreArgument()->vider();

    const SymboleFonctionLocale* symboleFonction = obtenirFonctionLocale(nomFonction);
    llvm::Function* fonctionCible = symboleFonction->fonction;
    llvm::FunctionType* typeFonction = fonctionCible->getFunctionType();
    
    unsigned int indexParam = 0; 
    for (INoeud* argumentEnfant : noeudAppelFonction->getEnfants()) 
    {
        argumentEnfant->accept(_visiteurGeneralCodeGen);
        llvm::Value* valeurArgument = getContextGenCode()->getValeurTemporaire().getAdresse();

        if (valeurArgument == nullptr) {
            throw std::runtime_error("Erreur : L'argument n'a pas généré de valeur.");
        }

        llvm::Value* valeurFinale = valeurArgument;
        if (indexParam < typeFonction->getNumParams()) {
            llvm::Type* typeAttendu = typeFonction->getParamType(indexParam);
            valeurFinale = getContextGenCode()->getBackend()->creerAutoCast(valeurArgument, typeAttendu);
        }

        getContextGenCode()->getRegistreArgument()->ajouter(valeurFinale);
        indexParam++;
    }

    std::vector<llvm::Value*> args = getContextGenCode()->getRegistreArgument()->recuperer();
    getContextGenCode()->getRegistreArgument()->vider();

    if(fonctionCible->getReturnType()->isVoidTy())
    {
        getContextGenCode()->getBackend()->getBuilder().CreateCall(fonctionCible, args);
        getContextGenCode()->modifierValeurTemporaire(Symbole(nullptr, getContextGenCode()->getValeurTemporaire().getType(), getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
        getContextGenCode()->modifierValeurTemporaire(Symbole(getContextGenCode()->getValeurTemporaire().getAdresse(), nullptr, getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
        return;
    }
    
    getContextGenCode()->modifierValeurTemporaire(Symbole(getContextGenCode()->getBackend()->getBuilder().CreateCall(
        fonctionCible, 
        args, 
        "resultat_appel"
    ), getContextGenCode()->getValeurTemporaire().getType(), getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
    getContextGenCode()->modifierValeurTemporaire(Symbole(getContextGenCode()->getValeurTemporaire().getAdresse(), symboleFonction->typeRetour, getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
}


// Gestion des fonctions Natives (Built-ins)

bool RegistreBuiltIns::estBuiltIn(const std::string& nom) {
    return nom == "print";
}

void RegistreBuiltIns::genererAppel(const std::string& nom, NoeudAppelFonction* noeudAppelFonction, ContextGenCode* context, IVisiteur* visiteur) {
    if (nom == "print") {
        if (noeudAppelFonction->getEnfants().empty()) {
            return;
        }

        noeudAppelFonction->getEnfants()[0]->accept(visiteur);
        llvm::Value* valeurArgument = context->getValeurTemporaire().getAdresse();
        IType* typeArgument = context->getValeurTemporaire().getType();
        
        if (valeurArgument == nullptr) {
            throw std::runtime_error("Erreur : L'argument de print n'a pas généré de valeur.");
        }

        char tag = 'i';
        auto& builder = context->getBackend()->getBuilder();

        if (typeArgument != nullptr && typeArgument->estFlottant()) {
            tag = 'f';
            valeurArgument = builder.CreateFPExt(valeurArgument, builder.getDoubleTy());
        } 
        else if (typeArgument != nullptr && typeArgument->estBooleen()) {
            tag = 'b';
            valeurArgument = builder.CreateZExt(valeurArgument, builder.getInt32Ty());
        } 
        else if (typeArgument != nullptr && typeArgument->estChaine()) {
            tag = 's';
            if (auto* loadInst = llvm::dyn_cast<llvm::LoadInst>(valeurArgument)) {
                valeurArgument = loadInst->getPointerOperand();
            }
        }

        llvm::Value* llvmTag = builder.getInt32(static_cast<uint32_t>(tag));
        
        const auto& symbolePtr = context->getRegistreFonctionLocale()->recuperer("print");
        const auto* symbolePrint = static_cast<const SymboleFonctionLocale*>(symbolePtr.get()); // nolint(cppcoreguidelines-pro-type-static-cast-downcast)
        
        builder.CreateCall(symbolePrint->fonction, { llvmTag, valeurArgument });
        context->modifierValeurTemporaire(Symbole(nullptr, context->getValeurTemporaire().getType(), context->getValeurTemporaire().getTypePointeElement()));
    }
}
