#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <memory>
#include <utility>

GestionFonction::GestionFonction(shared_ptr<ContextGenCode> contextGenCode, NoeudDeclarationFonction* noeudDeclarationFonction, IVisiteur* visiteurGeneralCodeGen) 
:   _contextGenCode(std::move(contextGenCode)),
    _noeudDeclarationFonction(noeudDeclarationFonction),
    enfants(&noeudDeclarationFonction->getEnfants()),
    _visiteurGeneralCodeGen(visiteurGeneralCodeGen)
{
}

GestionFonction::GestionFonction(shared_ptr<ContextGenCode> contextGenCode, IVisiteur* visiteurGeneralCodeGen)
:   _contextGenCode(std::move(contextGenCode)),
    _noeudDeclarationFonction(nullptr),
    enfants(nullptr),
    _visiteurGeneralCodeGen(visiteurGeneralCodeGen)
{
}


GestionFonction::~GestionFonction()
{}

GestionFonction::ArgumentsCodeGen GestionFonction::chargerArguments() 
{
    std::vector<llvm::Type*> argTypes;
    std::vector<NoeudArgFonction*> arguments;
    
    // Remplir le vecteur argTypes  et arguments
    for (const std::shared_ptr<INoeud> & index : *enfants) {
        INoeud* enfant = index.get();
        if (typeid(*enfant) == typeid(NoeudArgFonction)) {
            auto* arg = static_cast<NoeudArgFonction*>(enfant);
            arguments.push_back(arg);
            llvm::Type* argType = arg->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
            argTypes.push_back(argType);
        }
    }
    
    return ArgumentsCodeGen{argTypes, arguments};
}

llvm::Function* GestionFonction::creerFonction(llvm::Type* typeDeRetour, const ArgumentsCodeGen& argumentsCodeGen)
{
    llvm::FunctionType* funcType = llvm::FunctionType::get(typeDeRetour, argumentsCodeGen.argTypes, false);

    llvm::Function* function = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        _noeudDeclarationFonction->getNom(),
        &_contextGenCode->backend->getModule()
    );

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "entry", function);
    _contextGenCode->backend->getBuilder().SetInsertPoint(entryBlock);

    return function;
}

void GestionFonction::enregistrerFonction(llvm::Function* function)
{
    _contextGenCode->registreFonction->enregistrer(_noeudDeclarationFonction->getNom(), function);
}


void GestionFonction::initialiserContexte()
{
    _contextGenCode->returnContextCompilation->piler(_noeudDeclarationFonction->getTypeRetour());
    _contextGenCode->registreVariable->piler();
}


void GestionFonction::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& argumentsCodeGen)
{
    size_t argIndex = 0;
    for (auto* noeudArg : argumentsCodeGen.arguments) {
        llvm::Argument* arg = function->getArg(argIndex);
        arg->setName(noeudArg->getNom());
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = _contextGenCode->backend->getBuilder().CreateAlloca(argType);
        _contextGenCode->backend->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = noeudArg->getNom();
        argumentToken.type = TOKEN_IDENTIFIANT;
        
        _contextGenCode->registreVariable->enregistrer(argumentToken, alloca);
        
        argIndex++;
    }
}


void GestionFonction::traiterCorpsFonction()
{
    for (const auto & index : *enfants) {
        INoeud* enfant = index.get();
        if (typeid(*enfant) != typeid(NoeudArgFonction)) {
            index->accept(_visiteurGeneralCodeGen);
        }
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
    llvm::Function* fonctionCible = _contextGenCode->registreFonction->recuperer(nomFonction);

    llvm::FunctionType* typeFonction = fonctionCible->getFunctionType();
    

    unsigned int indexParam = 0; 

    for (const std::shared_ptr<INoeud>& argumentEnfant : noeudAppelFonction->getEnfants()) 
    {

        argumentEnfant->accept(_visiteurGeneralCodeGen);
        llvm::Value* valeurArgument = _contextGenCode->valeurTemporaire;

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


llvm::Function* GestionFonction::obtenirFonction(const std::string& nomFonction)
{
    llvm::Function* fonction = _contextGenCode->registreFonction->recuperer(nomFonction);

    if (fonction == nullptr) {
        throw std::runtime_error("Fonction introuvable : " + nomFonction);
    }

    return fonction;
}


void GestionFonction::genererAppelFonction(llvm::Function* fonction)
{
    std::vector<llvm::Value*> args = *_contextGenCode->registreArgument->recuperer();
    _contextGenCode->registreArgument->vider();

    _contextGenCode->valeurTemporaire = _contextGenCode->backend->getBuilder().CreateCall(
        fonction, 
        args, 
        "resultat_appel"
    );
}

void GestionFonction::declarerFonction()
{
    llvm::Type* typeDeRetour = _noeudDeclarationFonction->getTypeRetour()->genererTypeLLVM(_contextGenCode->backend->getContext());
    
    GestionFonction::ArgumentsCodeGen argumentsCodeGen = chargerArguments();

    // Étapes logiques de génération
    llvm::Function* function = creerFonction(typeDeRetour, argumentsCodeGen);
    enregistrerFonction(function);
    initialiserContexte();
    traiterArgumentsConstruit(function, argumentsCodeGen);
    traiterCorpsFonction();
    finaliserContexte();

    _contextGenCode->valeurTemporaire = function;
}

void GestionFonction::genererAppelFonction(NoeudAppelFonction* noeudAppelFonction)
{
    passArguments(noeudAppelFonction);
    llvm::Function* fonction = obtenirFonction(noeudAppelFonction->getNomFonction().value);
    genererAppelFonction(fonction);
}
