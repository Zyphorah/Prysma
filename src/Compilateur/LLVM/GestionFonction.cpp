#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/GestionFonction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include "Compilateur/Visiteur/Extracteurs/ExtracteurArgFonction.h"
#include <llvm/IR/Instructions.h>


// Génération de la Déclaration

std::unique_ptr<GenerateurDeclarationFonction> GenerateurDeclarationFonction::creer(ContextGenCode* context, NoeudDeclarationFonction* noeud, IVisiteur* visiteur)
{
    if (!context->nomClasseCourante.empty()) {
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
    std::string nomFonction = _noeudDeclarationFonction->getNom();
    
    const auto& symbolePtr = _contextGenCode->registreFonctionLocale->recuperer(nomFonction);
    const SymboleFonctionLocale* symbole = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    
    llvm::Function* function = symbole->fonction;

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "entry", function);
    _contextGenCode->backend->getBuilder().SetInsertPoint(entryBlock);

    return function;
}

llvm::Function* GenerateurDeclarationMethode::creerFonction()
{
    std::string nomFonction = _noeudDeclarationFonction->getNom();
    std::string nomClasse = _contextGenCode->nomClasseCourante;
    auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
    const auto& symbolePtr = classInfo->registreFonctionLocale->recuperer(nomFonction);
    const SymboleFonctionLocale* symbole = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    
    llvm::Function* function = symbole->fonction;

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "entry", function);
    _contextGenCode->backend->getBuilder().SetInsertPoint(entryBlock);

    return function;
}

void GenerateurDeclarationMethode::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args)
{
    size_t argIndex = 0;
    
    if (function->arg_size() > 0) {
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
        argIndex = 1;
    }

    for (auto* noeudArg : args.arguments) {
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

void GenerateurDeclarationStandard::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args)
{
    size_t argIndex = 0;

    for (auto* noeudArg : args.arguments) {
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

void GenerateurDeclarationFonction::declarerFonction()
{
    llvm::Type* typeDeRetour = _noeudDeclarationFonction->getTypeRetour()->genererTypeLLVM(_contextGenCode->backend->getContext());
    
    ArgumentsCodeGen argumentsCodeGen;
    if (_noeudDeclarationFonction != nullptr) {
        ExtracteurArgFonction extracteur;
        for (INoeud* noeud : _noeudDeclarationFonction->getArguments()) {
            extracteur.arg = nullptr;
            noeud->accept(&extracteur);
            if (extracteur.arg != nullptr) {
                argumentsCodeGen.arguments.push_back(extracteur.arg);
                llvm::Type* argType = extracteur.arg->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
                argumentsCodeGen.argTypes.push_back(argType);
            }
        }
    }

    llvm::Function* function = creerFonction();
    
    _contextGenCode->returnContextCompilation->piler(_noeudDeclarationFonction->getTypeRetour());
    _contextGenCode->registreVariable->piler();
    
    traiterArgumentsConstruit(function, argumentsCodeGen);

    _noeudDeclarationFonction->getCorps()->accept(_visiteurGeneralCodeGen);

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

    _contextGenCode->registreVariable->depiler();
    _contextGenCode->returnContextCompilation->depiler();

    _contextGenCode->valeurTemporaire.adresse = function;
}

// Génération de l'Appel

std::unique_ptr<GenerateurAppelFonction> GenerateurAppelFonction::creer(ContextGenCode* context, IVisiteur* visiteur)
{
    if (!context->nomClasseCourante.empty()) {
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
    std::string nomClasse = _contextGenCode->nomClasseCourante;
    auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
    if(classInfo->registreFonctionLocale->existe(nomFonction)){
        const auto& symbolePtr = classInfo->registreFonctionLocale->recuperer(nomFonction);
        return static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    }
    
    if (_contextGenCode->registreFonctionLocale->existe(nomFonction)) {
        const auto& symbolePtr = _contextGenCode->registreFonctionLocale->recuperer(nomFonction);
        return static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    }

    throw std::runtime_error("Méthode ou fonction introuvable dans la portée de la classe : " + nomFonction);
}

const SymboleFonctionLocale* GenerateurAppelStandard::obtenirFonctionLocale(const std::string& nomFonction)
{
    if (_contextGenCode->registreFonctionLocale->existe(nomFonction)) {
        const auto& symbolePtr = _contextGenCode->registreFonctionLocale->recuperer(nomFonction);
        return static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    }

    throw std::runtime_error("Fonction introuvable dans le registre local : " + nomFonction);
}

void GenerateurAppelFonction::genererAppelFonction(NoeudAppelFonction* noeudAppelFonction)
{
    std::string nomFonction = noeudAppelFonction->getNomFonction().value;

    if (RegistreBuiltIns::estBuiltIn(nomFonction)) {
        RegistreBuiltIns::genererAppel(nomFonction, noeudAppelFonction, _contextGenCode, _visiteurGeneralCodeGen);
        return; 
    }

    _contextGenCode->registreArgument->vider();

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

    std::vector<llvm::Value*> args = _contextGenCode->registreArgument->recuperer();
    _contextGenCode->registreArgument->vider();

    if(fonctionCible->getReturnType()->isVoidTy())
    {
        _contextGenCode->backend->getBuilder().CreateCall(fonctionCible, args);
        _contextGenCode->valeurTemporaire.adresse = nullptr;
        _contextGenCode->valeurTemporaire.type = nullptr;
        return;
    }
    
    _contextGenCode->valeurTemporaire.adresse = _contextGenCode->backend->getBuilder().CreateCall(
        fonctionCible, 
        args, 
        "resultat_appel"
    );
    _contextGenCode->valeurTemporaire.type = symboleFonction->typeRetour;
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
        llvm::Value* valeurArgument = context->valeurTemporaire.adresse;
        IType* typeArgument = context->valeurTemporaire.type;
        
        if (valeurArgument == nullptr) {
            throw std::runtime_error("Erreur : L'argument de print n'a pas généré de valeur.");
        }

        char tag = 'i';
        auto& builder = context->backend->getBuilder();

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
        
        const auto& symbolePtr = context->registreFonctionLocale->recuperer("print");
        const SymboleFonctionLocale* symbolePrint = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
        
        builder.CreateCall(symbolePrint->fonction, { llvmTag, valeurArgument });
        context->valeurTemporaire.adresse = nullptr;
    }
}
