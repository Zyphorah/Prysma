#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"
#include "Compilateur/AST/Noeuds/NoeudScope.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

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
    
    // Utiliser directement la liste stricte d'arguments
    for (NoeudArgFonction* arg : _noeudDeclarationFonction->getArguments()) {
        arguments.push_back(arg);
        llvm::Type* argType = arg->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
        argTypes.push_back(argType);
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
    SymboleFonction symboleFonction;
    symboleFonction.fonction = function;
    symboleFonction.typeRetour = _noeudDeclarationFonction->getTypeRetour();
    symboleFonction.noeud = _noeudDeclarationFonction;
  
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


void GestionFonction::traiterCorpsFonction()
{
    NoeudScope* corps = _noeudDeclarationFonction->getCorps();
    if (corps != nullptr) {
        corps->accept(_visiteurGeneralCodeGen);
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
    SymboleFonction symboleFonction = _contextGenCode->registreFonction->recuperer(nomFonction);
    llvm::Function* fonctionCible = symboleFonction.fonction;

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


SymboleFonction GestionFonction::obtenirFonction(const std::string& nomFonction)
{
    SymboleFonction symboleFonction = _contextGenCode->registreFonction->recuperer(nomFonction);

    if (symboleFonction.fonction == nullptr) {
        throw std::runtime_error("Fonction introuvable : " + nomFonction);
    }

    return symboleFonction;
}


void GestionFonction::genererAppelFonction(SymboleFonction symboleFonction)
{
    std::vector<llvm::Value*> args = _contextGenCode->registreArgument->recuperer();
    _contextGenCode->registreArgument->vider();

    llvm::Function* fonction = symboleFonction.fonction;
    
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
    _contextGenCode->valeurTemporaire.type = symboleFonction.typeRetour;
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
    SymboleFonction symboleFonction = obtenirFonction(nomFonction);
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
    }

    llvm::Value* llvmTag = builder.getInt8(static_cast<uint8_t>(tag));
    SymboleFonction symbolePrint = obtenirFonction("print");
    builder.CreateCall(symbolePrint.fonction, { llvmTag, valeurArgument });
    
    _contextGenCode->valeurTemporaire.adresse = nullptr;
}
