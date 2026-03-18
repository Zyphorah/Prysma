#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include "Compilateur/Visiteur/Extracteurs/ExtracteurArgFonction.h"
#include <llvm/IR/Instructions.h>

GestionFonction::GestionFonction(ContextGenCode* contextGenCode, NoeudDeclarationFonction* noeudDeclarationFonction, IVisiteur* visiteurGeneralCodeGen) 
:   _contextGenCode(contextGenCode),
    _noeudDeclarationFonction(noeudDeclarationFonction),
    _visiteurGeneralCodeGen(visiteurGeneralCodeGen)
{
}


GestionFonction::~GestionFonction()
{}

GestionFonction::ArgumentsCodeGen GestionFonction::chargerArguments() 
{
    std::vector<llvm::Type*> argTypes;
    std::vector<NoeudArgFonction*> arguments;
    
    ExtracteurArgFonction extracteur;
    for (INoeud* noeud : _noeudDeclarationFonction->getArguments()) {
        extracteur.arg = nullptr;
        noeud->accept(&extracteur);
        
        if (extracteur.arg != nullptr) {
            arguments.push_back(extracteur.arg);
            llvm::Type* argType = extracteur.arg->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
            argTypes.push_back(argType);
        }
    }
    
    return ArgumentsCodeGen{argTypes, arguments};
}

llvm::Function* GestionFonction::creerFonction()
{
    std::string nomFonction = _noeudDeclarationFonction->getNom();
    const SymboleFonctionLocale* symbole = nullptr;

    if (!_contextGenCode->nomClasseCourante.empty()) {
        std::string nomClasse = _contextGenCode->nomClasseCourante;
        auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
        const auto& symbolePtr = classInfo->registreFonctionLocale->recuperer(nomFonction);
        symbole = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    } else {
        const auto& symbolePtr = _contextGenCode->registreFonctionLocale->recuperer(nomFonction);
        symbole = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    }

    llvm::Function* function = symbole->fonction;

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "entry", function);
    _contextGenCode->backend->getBuilder().SetInsertPoint(entryBlock);

    return function;
}


void GestionFonction::initialiserContexte()
{
    _contextGenCode->returnContextCompilation->piler(_noeudDeclarationFonction->getTypeRetour());
    _contextGenCode->registreVariable->piler();
}


void GestionFonction::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& argumentsCodeGen)
{
    size_t argIndex = 0;
    
    // Si nous sommes dans une méthode de classe, le premier argument LLVM est 'this'
    bool estMethode = !_contextGenCode->nomClasseCourante.empty();
    
    if (estMethode && function->arg_size() > 0) {
        // Traiter le paramètre 'this' implicite
        llvm::Argument* thisArg = function->getArg(0);
        thisArg->setName("this");
        
        llvm::Type* argType = thisArg->getType();
        llvm::AllocaInst* alloca = _contextGenCode->backend->getBuilder().CreateAlloca(argType);
        _contextGenCode->backend->getBuilder().CreateStore(thisArg, alloca);
        
        Token thisToken;
        thisToken.value = "this";
        thisToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symboleThis;
        symboleThis.adresse = alloca;

        symboleThis.type = nullptr; 
        
        _contextGenCode->registreVariable->enregistrer(thisToken, symboleThis);
        
        // Décaler l'index des arguments explicites
        argIndex = 1;
    }

    for (auto* noeudArg : argumentsCodeGen.arguments) {
        llvm::Argument* arg = function->getArg(static_cast<unsigned int>(argIndex));
        arg->setName(noeudArg->getNom());
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = _contextGenCode->backend->getBuilder().CreateAlloca(argType);
        _contextGenCode->backend->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = noeudArg->getNom();
        argumentToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symbole;
        symbole.adresse = alloca;
        symbole.type = noeudArg->getType();
        _contextGenCode->registreVariable->enregistrer(argumentToken, symbole);
        
        argIndex++;
    }
}


void GestionFonction::finaliserContexte()
{
    _contextGenCode->registreVariable->depiler();
    _contextGenCode->returnContextCompilation->depiler();
}


void GestionFonction::passArguments(NoeudAppelFonction* noeudAppelFonction)
{
    _contextGenCode->registreArgument->vider();

    std::string nomFonction = noeudAppelFonction->getNomFonction().value;
    const SymboleFonctionLocale* symboleFonction = obtenirFonctionLocale(nomFonction);
    llvm::Function* fonctionCible = symboleFonction->fonction;

    llvm::FunctionType* typeFonction = fonctionCible->getFunctionType();
    

    unsigned int indexParam = 0; 

    for (INoeud* argumentEnfant : noeudAppelFonction->getEnfants()) 
    {

        argumentEnfant->accept(_visiteurGeneralCodeGen);
        llvm::Value* valeurArgument = _contextGenCode->valeurTemporaire.adresse;

        if (valeurArgument == nullptr) {
            throw std::runtime_error("Erreur : L'argument n'a pas généré de valeur.");
        }

        llvm::Value* valeurFinale = valeurArgument;

        if (indexParam < typeFonction->getNumParams()) {
            
            llvm::Type* typeAttendu = typeFonction->getParamType(indexParam);
            valeurFinale = _contextGenCode->backend->creerAutoCast(valeurArgument, typeAttendu);
        }

        _contextGenCode->registreArgument->ajouter(valeurFinale);
        
        indexParam++;
    }
}


const SymboleFonctionLocale* GestionFonction::obtenirFonctionLocale(const std::string& nomFonction)
{
    const SymboleFonctionLocale* symboleFonction = nullptr;

    if (!_contextGenCode->nomClasseCourante.empty()) {
        std::string nomClasse = _contextGenCode->nomClasseCourante;
        auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
        if(classInfo->registreFonctionLocale->existe(nomFonction)){
            const auto& symbolePtr = classInfo->registreFonctionLocale->recuperer(nomFonction);
            symboleFonction = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
        }
    } 

    if (symboleFonction == nullptr) {
        if (_contextGenCode->registreFonctionLocale->existe(nomFonction)) {
            const auto& symbolePtr = _contextGenCode->registreFonctionLocale->recuperer(nomFonction);
            symboleFonction = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
        }
    }

    if (symboleFonction == nullptr || symboleFonction->fonction == nullptr) {
        throw std::runtime_error("Fonction introuvable dans le registre local : " + nomFonction);
    }

    return symboleFonction;
}


void GestionFonction::genererAppelFonction(const SymboleFonctionLocale* symboleFonction)
{
    std::vector<llvm::Value*> args = _contextGenCode->registreArgument->recuperer();
    _contextGenCode->registreArgument->vider();

    llvm::Function* fonction = symboleFonction->fonction;
    
    if(fonction->getReturnType()->isVoidTy())
    {
        _contextGenCode->backend->getBuilder().CreateCall(fonction, args);
        _contextGenCode->valeurTemporaire.adresse = nullptr;
        _contextGenCode->valeurTemporaire.type = nullptr;
        return;
    }
    _contextGenCode->valeurTemporaire.adresse = _contextGenCode->backend->getBuilder().CreateCall(
        fonction, 
        args, 
        "resultat_appel"
    );
    _contextGenCode->valeurTemporaire.type = symboleFonction->typeRetour;
}

void GestionFonction::declarerFonction()
{
    llvm::Type* typeDeRetour = _noeudDeclarationFonction->getTypeRetour()->genererTypeLLVM(_contextGenCode->backend->getContext());
    
    GestionFonction::ArgumentsCodeGen argumentsCodeGen = chargerArguments();

    // Étapes logiques de génération

    // retirer les args
    llvm::Function* function = creerFonction();
    initialiserContexte();
    traiterArgumentsConstruit(function, argumentsCodeGen);

    _noeudDeclarationFonction->getCorps()->accept(_visiteurGeneralCodeGen);

    // Si la fonction est void et que le bloc courant n'a pas de terminateur,
    // on insère un ret void pour que le code LLVM IR soit valide.
    llvm::BasicBlock* blocCourant = _contextGenCode->backend->getBuilder().GetInsertBlock();
    if (blocCourant != nullptr && typeDeRetour->isVoidTy())
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
        if (blocCourant->getTerminator() == nullptr)
        {
            _contextGenCode->backend->getBuilder().CreateRetVoid();
        }
#pragma GCC diagnostic pop
    }

    finaliserContexte();

    _contextGenCode->valeurTemporaire.adresse = function;
}

void GestionFonction::genererAppelFonction(NoeudAppelFonction* noeudAppelFonction)
{
    std::string nomFonction = noeudAppelFonction->getNomFonction().value;

    if (nomFonction == "print") {
        genererBuiltInPrint(noeudAppelFonction);
        return; 
    }

    passArguments(noeudAppelFonction);
    const SymboleFonctionLocale* symboleFonction = obtenirFonctionLocale(nomFonction);
    genererAppelFonction(symboleFonction);
}

void GestionFonction::genererBuiltInPrint(NoeudAppelFonction* noeudAppelFonction)
{
    if (noeudAppelFonction->getEnfants().empty()) {
        return;
    }

    noeudAppelFonction->getEnfants()[0]->accept(_visiteurGeneralCodeGen);
    llvm::Value* valeurArgument = _contextGenCode->valeurTemporaire.adresse;
    IType* typeArgument = _contextGenCode->valeurTemporaire.type;
    
    if (valeurArgument == nullptr) {
        throw std::runtime_error("Erreur : L'argument de print n'a pas généré de valeur.");
    }

    char tag = 'i';
    auto& builder = _contextGenCode->backend->getBuilder();

    // Déterminer le type de l'argument via le type Prysma
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
    const SymboleFonctionLocale* symbolePrint = obtenirFonctionLocale("print");
    builder.CreateCall(symbolePrint->fonction, { llvmTag, valeurArgument });
    
    _contextGenCode->valeurTemporaire.adresse = nullptr;
}
