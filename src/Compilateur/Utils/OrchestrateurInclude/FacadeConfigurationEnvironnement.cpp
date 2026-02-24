#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"

#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreStrategieEquation.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"

// Stratégies d'équation
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieLitteral.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieUnRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieNegation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieString.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.h"

// Parseurs d'instructions
#include "Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Boucle/ParseurWhile.h"

#include "Compilateur/Lexer/TokenType.h"

#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>

FacadeConfigurationEnvironnement::FacadeConfigurationEnvironnement(RegistreFonction* registreFonctionGlobale)
    : _registreFonctionGlobale(registreFonctionGlobale),
      _registreStrategieEquation(nullptr),
      _constructeurArbreInstruction(nullptr),
      _constructeurEquation(nullptr),
      _parseurType(nullptr)
{
}

void FacadeConfigurationEnvironnement::initialiser()
{
    creerRegistres();
    creerContexte();
    enregistrerFonctionsExternes();
    enregistrerTypesDeBase();
    enregistrerStrategiesEquation();
    enregistrerInstructions();
}

void FacadeConfigurationEnvironnement::creerRegistres()
{
    _backend = std::make_unique<LlvmBackend>();
    _registreInstruction = std::make_unique<RegistreInstruction>();
    _registreVariable = std::make_unique<RegistreVariable>();
    _registreFonctionLocale = std::make_unique<RegistreFonction>();
    _registreType = std::make_unique<RegistreType>();
    _returnContextCompilation = std::make_unique<RetourContexteCompilation>();
    _registreArgument = std::make_unique<RegistreArgument>();
}

void FacadeConfigurationEnvironnement::creerContexte()
{
    Symbole valeurTemporaire;
    valeurTemporaire.adresse = nullptr;
    valeurTemporaire.type = nullptr;

    _context = std::make_unique<ContextGenCode>(
        _backend.get(),
        _registreInstruction.get(),
        _registreVariable.get(),
        _registreFonctionGlobale,
        _registreFonctionLocale.get(),
        _registreType.get(),
        _returnContextCompilation.get(),
        _registreArgument.get(),
        valeurTemporaire,
        &_arena
    );
}

void FacadeConfigurationEnvironnement::enregistrerFonctionsExternes()
{
    // Créer un vrai type Void pour Prysma
    IType* typeVoidPrysma = new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::Type::getVoidTy(_context->backend->getContext()));

    // backSlashN
    _context->backend->declarerExterne("backSlashN", llvm::Type::getVoidTy(_context->backend->getContext()), {});
    {
        auto symBackSlashNGlobal = std::make_unique<SymboleFonctionGlobale>();
        symBackSlashNGlobal->typeRetour = typeVoidPrysma;
        symBackSlashNGlobal->noeud = nullptr;
        _context->registreFonctionGlobale->enregistrer("backSlashN", std::move(symBackSlashNGlobal));

        auto symBackSlashNLocal = std::make_unique<SymboleFonctionLocale>();
        symBackSlashNLocal->fonction = _context->backend->getModule().getFunction("backSlashN");
        symBackSlashNLocal->typeRetour = typeVoidPrysma;
        symBackSlashNLocal->noeud = nullptr;
        _context->registreFonctionLocale->enregistrer("backSlashN", std::move(symBackSlashNLocal));
    }

    // print
    std::vector<llvm::Type*> print_args;
    print_args.push_back(llvm::Type::getInt8Ty(_context->backend->getContext()));
    llvm::FunctionType* print_type = llvm::FunctionType::get(llvm::Type::getVoidTy(_context->backend->getContext()), print_args, true);
    llvm::Function* printFunc = llvm::Function::Create(print_type, llvm::Function::ExternalLinkage, "print", _context->backend->getModule());
    {
        auto symPrintGlobal = std::make_unique<SymboleFonctionGlobale>();
        symPrintGlobal->typeRetour = typeVoidPrysma;
        symPrintGlobal->noeud = nullptr;
        _context->registreFonctionGlobale->enregistrer("print", std::move(symPrintGlobal));

        auto symPrintLocal = std::make_unique<SymboleFonctionLocale>();
        symPrintLocal->fonction = printFunc;
        symPrintLocal->typeRetour = typeVoidPrysma;
        symPrintLocal->noeud = nullptr;
        _context->registreFonctionLocale->enregistrer("print", std::move(symPrintLocal));
    }
}

void FacadeConfigurationEnvironnement::enregistrerTypesDeBase()
{
    _context->registreType->enregistrer(TOKEN_TYPE_STRING, llvm::Type::getInt8Ty(_context->backend->getContext()));
    _context->registreType->enregistrer(TOKEN_TYPE_CHAR, llvm::Type::getInt8Ty(_context->backend->getContext()));
    _context->registreType->enregistrer(TOKEN_TYPE_INT64, llvm::Type::getInt64Ty(_context->backend->getContext()));
    _context->registreType->enregistrer(TOKEN_TYPE_INT32, llvm::Type::getInt32Ty(_context->backend->getContext()));
    _context->registreType->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(_context->backend->getContext()));
    _context->registreType->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(_context->backend->getContext()));
    _context->registreType->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(_context->backend->getContext()));
}

void FacadeConfigurationEnvironnement::enregistrerStrategiesEquation()
{
    // Construire les chef d'orchestre de l'arbre syntaxique abstrait
    _constructeurArbreInstruction = new (_arena)
        ConstructeurArbreInstruction(_context->registreInstruction, _arena);

    _constructeurEquation = new (_arena)
        ConstructeurEquationFlottante(_constructeurArbreInstruction, _arena);

    // Créer le ParseurType avec le registre
    _parseurType = new (_arena.Allocate<ParseurType>())
        ParseurType(_context->registreType, _constructeurEquation->recupererConstructeurArbre());

    // Enregistrer les stratégies d'équation
    _registreStrategieEquation = new (_arena.Allocate<RegistreStrategieEquation>()) RegistreStrategieEquation();

    auto* stratLitInt = new (_arena.Allocate<StrategieLitteral>()) StrategieLitteral(_arena);
    _registreStrategieEquation->enregistrer(TOKEN_LIT_INT, stratLitInt);

    auto* stratLitFloat = new (_arena.Allocate<StrategieLitteral>()) StrategieLitteral(_arena);
    _registreStrategieEquation->enregistrer(TOKEN_LIT_FLOAT, stratLitFloat);

    auto* stratLitBool = new (_arena.Allocate<StrategieLitteral>()) StrategieLitteral(_arena);
    _registreStrategieEquation->enregistrer(TOKEN_LIT_BOLEEN, stratLitBool);

    auto* stratIdent = new (_arena.Allocate<StrategieIdentifiant>()) StrategieIdentifiant(_constructeurEquation);
    _registreStrategieEquation->enregistrer(TOKEN_IDENTIFIANT, stratIdent);

    auto* stratRef = new (_arena.Allocate<StrategieRef>()) StrategieRef(_arena);
    _registreStrategieEquation->enregistrer(TOKEN_REF, stratRef);

    auto* stratUnRef = new (_arena.Allocate<StrategieUnRef>()) StrategieUnRef(_arena);
    _registreStrategieEquation->enregistrer(TOKEN_UNREF, stratUnRef);

    auto* stratNeg = new (_arena.Allocate<StrategieNegation>()) StrategieNegation(_constructeurEquation);
    _registreStrategieEquation->enregistrer(TOKEN_NON, stratNeg);

    auto* stratTab = new (_arena.Allocate<StrategieTableauInitialisation>()) StrategieTableauInitialisation(_constructeurEquation->recupererConstructeurArbre());
    _registreStrategieEquation->enregistrer(TOKEN_CROCHET_OUVERT, stratTab);

    auto* stratString = new (_arena.Allocate<StrategieString>()) StrategieString(_arena);
    _registreStrategieEquation->enregistrer(TOKEN_GUILLEMET, stratString);

    // Ajouter la stratégie TOKEN_CALL
    auto* stratCall = new (_arena.Allocate<StrategieAppelFonction>()) StrategieAppelFonction(_constructeurEquation->recupererConstructeurArbre());
    _registreStrategieEquation->enregistrer(TOKEN_CALL, stratCall);

    ConstructeurEquationFlottante::setRegistreStrategieEquation(_registreStrategieEquation);
}

void FacadeConfigurationEnvironnement::enregistrerInstructions()
{
    auto* parsScope = new (_arena.Allocate<ParseurScope>()) ParseurScope(_constructeurArbreInstruction);
    _context->registreInstruction->enregistrer(TOKEN_SCOPE, parsScope);

    auto* parsFonc = new (_arena.Allocate<ParseurDeclarationFonction>()) ParseurDeclarationFonction(_constructeurArbreInstruction, _parseurType);
    _context->registreInstruction->enregistrer(TOKEN_FONCTION, parsFonc);

    auto* parsAff = new (_arena.Allocate<ParseurAffectationVariable>()) ParseurAffectationVariable(_context->backend, _context->registreVariable, _context->registreType, _constructeurEquation->recupererConstructeurArbre());
    _context->registreInstruction->enregistrer(TOKEN_AFF, parsAff);

    auto* parsDec = new (_arena.Allocate<ParseurDeclarationVariable>()) ParseurDeclarationVariable(_parseurType, _constructeurEquation->recupererConstructeurArbre());
    _context->registreInstruction->enregistrer(TOKEN_DEC, parsDec);

    auto* parsCall = new (_arena.Allocate<ParseurInstructionAppel>()) ParseurInstructionAppel(_constructeurEquation->recupererConstructeurArbre());
    _context->registreInstruction->enregistrer(TOKEN_CALL, parsCall);

    auto* parsRet = new (_arena.Allocate<ParseurRetour>()) ParseurRetour(_constructeurEquation->recupererConstructeurArbre());
    _context->registreInstruction->enregistrer(TOKEN_RETOUR, parsRet);

    auto* parsArg = new (_arena.Allocate<ParseurArgFonction>()) ParseurArgFonction(_parseurType, _arena);
    _context->registreInstruction->enregistrer(TOKEN_ARG, parsArg);

    auto* parsUnRef = new (_arena.Allocate<ParseurUnRefVariable>()) ParseurUnRefVariable(_arena);
    _context->registreInstruction->enregistrer(TOKEN_UNREF, parsUnRef);

    auto* parsRefVar = new (_arena.Allocate<ParseurRefVariable>()) ParseurRefVariable(_arena);
    _context->registreInstruction->enregistrer(TOKEN_REF, parsRefVar);

    auto* parsIf = new (_arena.Allocate<ParseurIf>()) ParseurIf(_constructeurEquation->recupererConstructeurArbre(), _constructeurArbreInstruction);
    _context->registreInstruction->enregistrer(TOKEN_SI, parsIf);

    auto* parsWhile = new (_arena.Allocate<ParseurWhile>()) ParseurWhile(_constructeurEquation->recupererConstructeurArbre(), _constructeurArbreInstruction);
    _context->registreInstruction->enregistrer(TOKEN_TANT_QUE, parsWhile);
}

ContextGenCode* FacadeConfigurationEnvironnement::getContext() const
{
    return _context.get();
}

llvm::BumpPtrAllocator& FacadeConfigurationEnvironnement::getArena()
{
    return _arena;
}

ConstructeurArbreInstruction* FacadeConfigurationEnvironnement::getConstructeurArbreInstruction() const
{
    return _constructeurArbreInstruction;
}

ConstructeurEquationFlottante* FacadeConfigurationEnvironnement::getConstructeurEquation() const
{
    return _constructeurEquation;
}
