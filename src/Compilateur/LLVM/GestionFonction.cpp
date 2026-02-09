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
            llvm::Type* argType = _contextGenCode->registreType->recuperer(arg->_type);
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
        _noeudDeclarationFonction->_nom,
        &_contextGenCode->backend->getModule()
    );

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "entry", function);
    _contextGenCode->backend->getBuilder().SetInsertPoint(entryBlock);

    return function;
}


void GestionFonction::enregistrerFonction(llvm::Function* function)
{
    _contextGenCode->registreFonction->enregistrer(_noeudDeclarationFonction->_nom, function);
}


void GestionFonction::initialiserContexte()
{
    _contextGenCode->returnContextCompilation->piler(_noeudDeclarationFonction->_typeRetourToken);
    _contextGenCode->registreVariable->piler();
}


void GestionFonction::traiterArguments(llvm::Function* function, const ArgumentsCodeGen& argumentsCodeGen)
{
    size_t argIndex = 0;
    for (auto* noeudArg : argumentsCodeGen.arguments) {
        llvm::Argument* arg = function->getArg(argIndex);
        arg->setName(noeudArg->_nom);
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = _contextGenCode->backend->getBuilder().CreateAlloca(argType);
        _contextGenCode->backend->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = noeudArg->_nom;
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


void GestionFonction::traiterArguments(NoeudAppelFonction* noeudAppelFonction)
{
    _contextGenCode->registreArgument->vider();
    
    for (const auto& argumentEnfant : noeudAppelFonction->getEnfants()) 
    {
        argumentEnfant->accept(_visiteurGeneralCodeGen);
        llvm::Value* valeurArgument = _contextGenCode->valeurTemporaire;

        if (valeurArgument == nullptr) {
            throw std::runtime_error("Erreur : L'argument passé à la fonction " + noeudAppelFonction->_nomFonction.value + " n'a pas généré de valeur.");
        }

        _contextGenCode->registreArgument->ajouter(valeurArgument);
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

    _contextGenCode->valeurTemporaire = _contextGenCode->backend->getBuilder().CreateCall(
        fonction, 
        args, 
        "resultat_appel"
    );
}

void GestionFonction::declarerFonction()
{
    llvm::Type* typeDeRetour = _contextGenCode->registreType->recuperer(_noeudDeclarationFonction->_typeRetourToken);
    
    GestionFonction::ArgumentsCodeGen argumentsCodeGen = chargerArguments();

    // Étapes logiques de génération
    llvm::Function* function = creerFonction(typeDeRetour, argumentsCodeGen);
    enregistrerFonction(function);
    initialiserContexte();
    traiterArguments(function, argumentsCodeGen);
    traiterCorpsFonction();
    finaliserContexte();

    _contextGenCode->valeurTemporaire = function;
}

void GestionFonction::genererAppelFonction(NoeudAppelFonction* noeudAppelFonction)
{
    traiterArguments(noeudAppelFonction);
    llvm::Function* fonction = obtenirFonction(noeudAppelFonction->_nomFonction.value);
    genererAppelFonction(fonction);
}
