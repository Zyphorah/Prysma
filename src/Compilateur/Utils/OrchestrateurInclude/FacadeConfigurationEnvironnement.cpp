#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/RegistreExpression.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/Instruction/ParseurDelete.h"
#include "Compilateur/Instruction/ParseurInclude.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"

// Expressions
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/Math/ExpressionLitteral.h"
#include "Compilateur/Registre/RegistreFichier.h"
#include "Compilateur/Variable/ExpressionIdentifiant.h"
#include "Compilateur/Variable/ExpressionRefVariable.h"
#include "Compilateur/Variable/ExpressionUnRefVariable.h"
#include "Compilateur/Math/ExpressionNegation.h"
#include "Compilateur/Math/ExpressionString.h"
#include "Compilateur/Tableau/ExpressionTableauInitialisation.h"
#include "Compilateur/objet/ExpressionAppelCentral.h"
#include "Compilateur/Instruction/ExpressionNew.h"

// Parseurs d'instructions
#include "Compilateur/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/Variable/ParseurAffectationVariable.h"
#include "Compilateur/objet/ParseurAppelCentral.h"
#include "Compilateur/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/Variable/ParseurRefVariable.h"
#include "Compilateur/Variable/ParseurUnRefVariable.h"
#include "Compilateur/Fonction/ParseurRetour.h"
#include "Compilateur/Fonction/ParseurArgFonction.h"
#include "Compilateur/Instruction/ParseurIf.h"
#include "Compilateur/Instruction/ParseurWhile.h"

#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/objet/ParseurClass.h"

#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/Support/Allocator.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

FacadeConfigurationEnvironnement::FacadeConfigurationEnvironnement(RegistreFonctionGlobale* registreFonctionGlobale, RegistreFichier* registreFichier)
    : _registreFonctionGlobale(registreFonctionGlobale),
      _registreFichier(registreFichier),
    _registreExpression(nullptr),
      _constructeurArbreInstruction(nullptr),
      _constructeurEquation(nullptr),
      _parseurType(nullptr),
    _contextParseur(nullptr),
    _contexteExpression(nullptr)
{
}

FacadeConfigurationEnvironnement::~FacadeConfigurationEnvironnement()
{
    if (_contexteExpression != nullptr) {
        _contexteExpression->~ContexteExpression();
    }
    if (_contextParseur != nullptr) {
        _contextParseur->~ContextParseur();
    }
    if (_parseurType != nullptr) {
        _parseurType->~ParseurType();
    }
    if (_constructeurEquation != nullptr) {
        _constructeurEquation->~ConstructeurEquationFlottante();
    }
    if (_constructeurArbreInstruction != nullptr) {
        _constructeurArbreInstruction->~ConstructeurArbreInstruction();
    }
    if (_registreExpression != nullptr) {
        _registreExpression->~RegistreExpression();
    }
}

void FacadeConfigurationEnvironnement::initialiser(const std::string& cheminFichier)
{
    creerRegistres();
    creerContexte(cheminFichier);
    enregistrerFonctionsExternes();
    enregistrerTypesDeBase();
    enregistrerExpressions();
    enregistrerInstructions();
}

void FacadeConfigurationEnvironnement::creerRegistres()
{
    _backend = std::make_unique<LlvmBackend>();
    _registreInstruction = std::make_unique<RegistreInstruction>();
    _registreVariable = std::make_unique<RegistreVariable>();
    _registreFonctionLocale = std::make_unique<RegistreFonctionLocale>();
    _registreType = std::make_unique<RegistreType>();
    _returnContextCompilation = std::make_unique<RetourContexteCompilation>();
    _registreArgument = std::make_unique<RegistreArgument>();
    _registreClass = std::make_unique<RegistreClass>();
}

void FacadeConfigurationEnvironnement::creerContexte(const std::string& cheminFichier)
{
    Symbole valeurTemporaire;
    valeurTemporaire = Symbole(nullptr, valeurTemporaire.getType(), valeurTemporaire.getTypePointeElement());
    valeurTemporaire = Symbole(valeurTemporaire.getAdresse(), nullptr, valeurTemporaire.getTypePointeElement());

    _context = std::make_unique<ContextGenCode>(
        _registreFichier,
        _backend.get(),
        _registreInstruction.get(),
        _registreVariable.get(),
        _registreFonctionGlobale,
        _registreFonctionLocale.get(),
        _registreType.get(),
        _returnContextCompilation.get(),
        _registreArgument.get(),
        _registreClass.get(),
        valeurTemporaire,
        &_arena,
        cheminFichier
    );
}

void FacadeConfigurationEnvironnement::enregistrerFonctionsExternes()
{
    // Créer un vrai type Void pour Prysma
    IType* typeVoidPrysma = new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::Type::getVoidTy(_context->getBackend()->getContext())); // NOLINT(cppcoreguidelines-owning-memory)

    // backSlashN
    _context->getBackend()->declarerExterne("backSlashN", llvm::Type::getVoidTy(_context->getBackend()->getContext()), {});
    {
        auto symBackSlashNGlobal = std::make_unique<SymboleFonctionGlobale>();
        symBackSlashNGlobal->typeRetour = typeVoidPrysma;
        symBackSlashNGlobal->noeud = nullptr;
        _context->getRegistreFonctionGlobale()->enregistrer("backSlashN", std::move(symBackSlashNGlobal));

        auto symBackSlashNLocal = std::make_unique<SymboleFonctionLocale>();
        symBackSlashNLocal->fonction = _context->getBackend()->getModule().getFunction("backSlashN");
        symBackSlashNLocal->typeRetour = typeVoidPrysma;
        symBackSlashNLocal->noeud = nullptr;
        _context->getRegistreFonctionLocale()->enregistrer("backSlashN", std::move(symBackSlashNLocal));
    }

    // print
    std::vector<llvm::Type*> print_args;
    print_args.push_back(llvm::Type::getInt32Ty(_context->getBackend()->getContext()));
    llvm::FunctionType* print_type = llvm::FunctionType::get(llvm::Type::getVoidTy(_context->getBackend()->getContext()), print_args, true);
    llvm::Function* printFunc = llvm::Function::Create(print_type, llvm::Function::ExternalLinkage, "print", _context->getBackend()->getModule());
    {
        auto symPrintGlobal = std::make_unique<SymboleFonctionGlobale>();
        symPrintGlobal->typeRetour = typeVoidPrysma;
        symPrintGlobal->noeud = nullptr;
        _context->getRegistreFonctionGlobale()->enregistrer("print", std::move(symPrintGlobal));

        auto symPrintLocal = std::make_unique<SymboleFonctionLocale>();
        symPrintLocal->fonction = printFunc;
        symPrintLocal->typeRetour = typeVoidPrysma;
        symPrintLocal->noeud = nullptr;
        _context->getRegistreFonctionLocale()->enregistrer("print", std::move(symPrintLocal));
    }

    // prysma_malloc
    std::vector<llvm::Type*> malloc_args;
    malloc_args.push_back(llvm::Type::getInt64Ty(_context->getBackend()->getContext()));
    llvm::FunctionType* malloc_type = llvm::FunctionType::get(llvm::PointerType::getUnqual(_context->getBackend()->getContext()), malloc_args, false);
    llvm::Function* mallocFunc = llvm::Function::Create(malloc_type, llvm::Function::ExternalLinkage, "prysma_malloc", _context->getBackend()->getModule());
    {
        auto symMallocGlobal = std::make_unique<SymboleFonctionGlobale>();
        symMallocGlobal->typeRetour = static_cast<TypeSimple*>(static_cast<void*>(new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::PointerType::getUnqual(_context->getBackend()->getContext()))));
        symMallocGlobal->noeud = nullptr;
        _context->getRegistreFonctionGlobale()->enregistrer("prysma_malloc", std::move(symMallocGlobal));

        auto symMallocLocal = std::make_unique<SymboleFonctionLocale>();
        symMallocLocal->fonction = mallocFunc;
        symMallocLocal->typeRetour = new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::PointerType::getUnqual(_context->getBackend()->getContext())); // NOLINT(cppcoreguidelines-owning-memory)
        symMallocLocal->noeud = nullptr;
        _context->getRegistreFonctionLocale()->enregistrer("prysma_malloc", std::move(symMallocLocal));
    }

    // prysma_free
    std::vector<llvm::Type*> free_args;
    free_args.push_back(llvm::PointerType::getUnqual(_context->getBackend()->getContext()));
    llvm::FunctionType* free_type = llvm::FunctionType::get(llvm::Type::getVoidTy(_context->getBackend()->getContext()), free_args, false);
    llvm::Function* freeFunc = llvm::Function::Create(free_type, llvm::Function::ExternalLinkage, "prysma_free", _context->getBackend()->getModule());
    {
        auto symFreeGlobal = std::make_unique<SymboleFonctionGlobale>();
        symFreeGlobal->typeRetour = typeVoidPrysma;
        symFreeGlobal->noeud = nullptr;
        _context->getRegistreFonctionGlobale()->enregistrer("prysma_free", std::move(symFreeGlobal));

        auto symFreeLocal = std::make_unique<SymboleFonctionLocale>();
        symFreeLocal->fonction = freeFunc;
        symFreeLocal->typeRetour = typeVoidPrysma;
        symFreeLocal->noeud = nullptr;
        _context->getRegistreFonctionLocale()->enregistrer("prysma_free", std::move(symFreeLocal));
    }
}

void FacadeConfigurationEnvironnement::enregistrerTypesDeBase()
{
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_STRING, llvm::Type::getInt8Ty(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_CHAR, llvm::Type::getInt8Ty(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_INT64, llvm::Type::getInt64Ty(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_INT32, llvm::Type::getInt32Ty(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(_context->getBackend()->getContext()));
    _context->getRegistreType()->enregistrer(TOKEN_TYPE_PTR, llvm::PointerType::getUnqual(_context->getBackend()->getContext()));
}

void FacadeConfigurationEnvironnement::creerContextParseur()
{
    if (_constructeurArbreInstruction == nullptr || _constructeurEquation == nullptr || _parseurType == nullptr) {
        throw std::logic_error("ContextParseur ne peut pas être créé avant les constructeurs d'arbres et le parseur de type");
    }

    ContextParseur::Dependencies deps = {
        _constructeurEquation->recupererConstructeurArbre(),
        _constructeurArbreInstruction,
        _parseurType,
        _registreVariable.get(),
        _registreType.get()
    };
    _contextParseur = new (_arena.Allocate<ContextParseur>()) ContextParseur(deps); // NOLINT(cppcoreguidelines-owning-memory)
}

void FacadeConfigurationEnvironnement::enregistrerExpressions()
{
    // Construire les chef d'orchestre de l'arbre syntaxique abstrait
    _constructeurArbreInstruction = new (_arena) // NOLINT(cppcoreguidelines-owning-memory)
        ConstructeurArbreInstruction(_registreInstruction.get(), _arena); 

    _registreExpression = new (_arena.Allocate<RegistreExpression>()) RegistreExpression(); // NOLINT(cppcoreguidelines-owning-memory)

    _constructeurEquation = new (_arena) // NOLINT(cppcoreguidelines-owning-memory)
        ConstructeurEquationFlottante(_registreExpression, _arena);

    // Créer le ParseurType avec le registre
    _parseurType = new (_arena.Allocate<ParseurType>()) // NOLINT(cppcoreguidelines-owning-memory)
        ParseurType(_context->getRegistreType(), _constructeurEquation->recupererConstructeurArbre());

    if (_contextParseur == nullptr) {
        creerContextParseur();
    }

    _contexteExpression = new (_arena.Allocate<ContexteExpression>()) ContexteExpression( // NOLINT(cppcoreguidelines-owning-memory)
        _constructeurEquation->recupererConstructeurArbre(),
        _constructeurArbreInstruction,
        _parseurType,
        _contextParseur,
        &_arena,
        _registreVariable.get(),
        _registreType.get()
    );

    auto* exprLitInt = new (_arena.Allocate<ExpressionLitteral>()) ExpressionLitteral(*_contexteExpression); // // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_LIT_INT, exprLitInt);

    auto* exprLitFloat = new (_arena.Allocate<ExpressionLitteral>()) ExpressionLitteral(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_LIT_FLOAT, exprLitFloat);

    auto* exprLitBool = new (_arena.Allocate<ExpressionLitteral>()) ExpressionLitteral(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_LIT_BOLEEN, exprLitBool);

    auto* exprIdentifiant = new (_arena.Allocate<ExpressionIdentifiant>()) ExpressionIdentifiant(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_IDENTIFIANT, exprIdentifiant);

    auto* exprRef = new (_arena.Allocate<ExpressionRefVariable>()) ExpressionRefVariable(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_REF, exprRef);

    auto* exprUnRef = new (_arena.Allocate<ExpressionUnRefVariable>()) ExpressionUnRefVariable(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_UNREF, exprUnRef);

    auto* exprNeg = new (_arena.Allocate<ExpressionNegation>()) ExpressionNegation(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_NON, exprNeg);

    auto* exprString = new (_arena.Allocate<ExpressionString>()) ExpressionString(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_GUILLEMET, exprString);

    auto* exprTab = new (_arena.Allocate<ExpressionTableauInitialisation>()) ExpressionTableauInitialisation(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_CROCHET_OUVERT, exprTab);

    auto* exprCall = new (_arena.Allocate<ExpressionAppelCentral>()) ExpressionAppelCentral(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_CALL, exprCall);

    auto* exprNew = new (_arena.Allocate<ExpressionNew>()) ExpressionNew(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registreExpression->enregistrer(TOKEN_NEW, exprNew);
}

void FacadeConfigurationEnvironnement::enregistrerInstructions()
{
    
    auto* parsFonc = new (_arena.Allocate<ParseurDeclarationFonction>()) ParseurDeclarationFonction(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_FONCTION, parsFonc);

    auto* parsAff = new (_arena.Allocate<ParseurAffectationVariable>()) ParseurAffectationVariable(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_AFF, parsAff);

    auto* parsDec = new (_arena.Allocate<ParseurDeclarationVariable>()) ParseurDeclarationVariable(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_DEC, parsDec);

    auto* parsCall = new (_arena.Allocate<ParseurAppelCentral>()) ParseurAppelCentral(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_CALL, parsCall);

    auto* parsRet = new (_arena.Allocate<ParseurRetour>()) ParseurRetour(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_RETOUR, parsRet);

    auto* parsArg = new (_arena.Allocate<ParseurArgFonction>()) ParseurArgFonction(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_ARG, parsArg);

    auto* parsUnRef = new (_arena.Allocate<ParseurUnRefVariable>()) ParseurUnRefVariable(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_UNREF, parsUnRef);

    auto* parsRefVar = new (_arena.Allocate<ParseurRefVariable>()) ParseurRefVariable(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_REF, parsRefVar);

    auto* parsIf = new (_arena.Allocate<ParseurIf>()) ParseurIf(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_SI, parsIf);

    auto* parsWhile = new (_arena.Allocate<ParseurWhile>()) ParseurWhile(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_TANT_QUE, parsWhile);

    auto* parsInclude = new (_arena.Allocate<ParseurInclude>()) ParseurInclude(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_INCLUDE, parsInclude);

    auto* parsDelete = new (_arena.Allocate<ParseurDelete>()) ParseurDelete(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_DELETE, parsDelete);

    auto* parsClass = new (_arena.Allocate<ParseurClass>()) ParseurClass(*_contextParseur); // NOLINT(cppcoreguidelines-owning-memory)
    _registreInstruction->enregistrer(TOKEN_CLASS, parsClass);
}

auto FacadeConfigurationEnvironnement::getContext() const -> ContextGenCode*
{
    return _context.get();
}

auto FacadeConfigurationEnvironnement::getArena() -> llvm::BumpPtrAllocator&
{
    return _arena;
}

auto FacadeConfigurationEnvironnement::getConstructeurArbreInstruction() const -> ConstructeurArbreInstruction*
{
    return _constructeurArbreInstruction;
}

auto FacadeConfigurationEnvironnement::getConstructeurEquation() const -> ConstructeurEquationFlottante*
{
    return _constructeurEquation;
}
// NOLINTEND(cppcoreguidelines-owning-memory)
