#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <memory>
#include <utility>


GenerateurFonction::GenerateurFonction(shared_ptr<ContextGenCode> contextGenCode, NoeudDeclarationFonction* noeudDeclarationFonction, IVisiteur* visiteurGeneralCodeGen) 
:   _contextGenCode(std::move(contextGenCode)),
    _noeudDeclarationFonction(noeudDeclarationFonction),
    enfants(_noeudDeclarationFonction->getEnfants()),
    _visiteurGeneralCodeGen(visiteurGeneralCodeGen)
{
}


GenerateurFonction::~GenerateurFonction()
{}

GenerateurFonction::ArgumentsCodeGen GenerateurFonction::chargerArguments() 
{
    std::vector<llvm::Type*> argTypes;
    std::vector<NoeudArgFonction*> arguments;
    
    // Remplir le vecteur argTypes  et arguments
    for (const std::shared_ptr<INoeud> & index : enfants) {
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



llvm::Function* GenerateurFonction::creerFonction(llvm::Type* typeDeRetour, const ArgumentsCodeGen& argumentsCodeGen)
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


void GenerateurFonction::enregistrerFonction(llvm::Function* function)
{
    _contextGenCode->registreFonction->enregistrer(_noeudDeclarationFonction->_nom, function);
}


void GenerateurFonction::initialiserContexte()
{
    _contextGenCode->returnContextCompilation->piler(_noeudDeclarationFonction->_typeRetourToken);
    _contextGenCode->registreVariable->piler();
}


void GenerateurFonction::traiterArguments(llvm::Function* function, const ArgumentsCodeGen& argumentsCodeGen)
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


void GenerateurFonction::traiterCorpsFonction()
{
    for (const auto & index : enfants) {
        INoeud* enfant = index.get();
        if (typeid(*enfant) != typeid(NoeudArgFonction)) {
            index->accept(_visiteurGeneralCodeGen);
        }
    }
}


void GenerateurFonction::finaliserContexte()
{
    _contextGenCode->registreVariable->depiler();
    _contextGenCode->returnContextCompilation->depiler();
}


void GenerateurFonction::declarerFonction()
{
    llvm::Type* typeDeRetour = _contextGenCode->registreType->recuperer(_noeudDeclarationFonction->_typeRetourToken);
    
    GenerateurFonction::ArgumentsCodeGen argumentsCodeGen = chargerArguments();

    // Étapes logiques de génération
    llvm::Function* function = creerFonction(typeDeRetour, argumentsCodeGen);
    enregistrerFonction(function);
    initialiserContexte();
    traiterArguments(function, argumentsCodeGen);
    traiterCorpsFonction();
    finaliserContexte();

    _contextGenCode->valeurTemporaire = function;
}
