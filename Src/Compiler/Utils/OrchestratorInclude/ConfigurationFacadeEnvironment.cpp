#include "Compiler/AST/Utils/OrchestratorInclude/ConfigurationFacadeEnvironment.h"

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Stack/ReturnContextCompilation.h"
#include "Compiler/AST/Registry/RegistryArgument.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/RegistryExpression.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Registry/RegistryInstruction.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include "Compiler/AST/BuilderTreeInstruction.h"
#include "Compiler/Instruction/ParserDelete.h"
#include "Compiler/Instruction/ParserInclude.h"
#include "Compiler/Builder/Equation/BuilderEquationFlottante.h"
#include "Compiler/Parser/ParserType.h"

// Expressions
#include "Compiler/LLVM/LlvmBackend.h"
#include "Compiler/Math/ExpressionLiteral.h"
#include "Compiler/Registry/RegistryFile.h"
#include "Compiler/Variable/ExpressionIdentifiant.h"
#include "Compiler/Variable/ExpressionRefVariable.h"
#include "Compiler/Variable/ExpressionUnRefVariable.h"
#include "Compiler/Math/ExpressionNegation.h"
#include "Compiler/Math/ExpressionString.h"
#include "Compiler/Array/ExpressionArrayInitialization.h"
#include "Compiler/Object/ExpressionCallCentral.h"
#include "Compiler/Instruction/ExpressionNew.h"

// Parsers d'instructions
#include "Compiler/Function/ParserDeclarationFunction.h"
#include "Compiler/Variable/ParserAssignmentVariable.h"
#include "Compiler/Object/ParserCallCentral.h"
#include "Compiler/Variable/ParserDeclarationVariable.h"
#include "Compiler/Variable/ParserRefVariable.h"
#include "Compiler/Variable/ParserUnRefVariable.h"
#include "Compiler/Function/ParserReturn.h"
#include "Compiler/Function/ParserArgFunction.h"
#include "Compiler/Instruction/ParserIf.h"
#include "Compiler/Instruction/ParserWhile.h"

#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Object/ParserClass.h"

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

ConfigurationFacadeEnvironnement::ConfigurationFacadeEnvironnement(RegistryFunctionGlobale* registryFunctionGlobale, RegistryFile* registryFile)
    : _registryFunctionGlobale(registryFunctionGlobale),
      _registryFile(registryFile),
    _registryExpression(nullptr),
      _builderTreeInstruction(nullptr),
      _builderEquation(nullptr),
      _parserType(nullptr),
    _contextParser(nullptr),
    _contexteExpression(nullptr)
{
}

ConfigurationFacadeEnvironnement::~ConfigurationFacadeEnvironnement()
{
    if (_contexteExpression != nullptr) {
        _contexteExpression->~ContextExpression();
    }
    if (_contextParser != nullptr) {
        _contextParser->~ContextParser();
    }
    if (_parserType != nullptr) {
        _parserType->~ParserType();
    }
    if (_builderEquation != nullptr) {
        _builderEquation->~BuilderEquationFlottante();
    }
    if (_builderTreeInstruction != nullptr) {
        _builderTreeInstruction->~BuilderTreeInstruction();
    }
    if (_registryExpression != nullptr) {
        _registryExpression->~RegistryExpression();
    }
}

void ConfigurationFacadeEnvironnement::initialiser(const std::string& cheminFile)
{
    creerRegistrys();
    creerContext(cheminFile);
    enregistryrFunctionsExternes();
    enregistryrTypesDeBase();
    enregistryrExpressions();
    enregistryrInstructions();
}

void ConfigurationFacadeEnvironnement::creerRegistrys()
{
    _backend = std::make_unique<LlvmBackend>();
    _registryInstruction = std::make_unique<RegistryInstruction>();
    _registryVariable = std::make_unique<RegistryVariable>();
    _registryFunctionLocale = std::make_unique<RegistryFunctionLocale>();
    _registryType = std::make_unique<RegistryType>();
    _returnContextCompilation = std::make_unique<ReturnContextCompilation>();
    _registryArgument = std::make_unique<RegistryArgument>();
    _registryClass = std::make_unique<RegistryClass>();
}

void ConfigurationFacadeEnvironnement::creerContext(const std::string& cheminFile)
{
    Symbole valeurTemporaire;
    valeurTemporaire = Symbole(nullptr, valeurTemporaire.getType(), valeurTemporaire.getTypePointeElement());
    valeurTemporaire = Symbole(valeurTemporaire.getAdresse(), nullptr, valeurTemporaire.getTypePointeElement());

    _context = std::make_unique<ContextGenCode>(
        _registryFile,
        _backend.get(),
        _registryInstruction.get(),
        _registryVariable.get(),
        _registryFunctionGlobale,
        _registryFunctionLocale.get(),
        _registryType.get(),
        _returnContextCompilation.get(),
        _registryArgument.get(),
        _registryClass.get(),
        valeurTemporaire,
        &_arena,
        cheminFile
    );
}

void ConfigurationFacadeEnvironnement::enregistryrFunctionsExternes()
{
    // Créer un vrai type Void pour Prysma
    IType* typeVoidPrysma = new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::Type::getVoidTy(_context->getBackend()->getContext())); // NOLINT(cppcoreguidelines-owning-memory)

    // backSlashN
    _context->getBackend()->declarerExterne("backSlashN", llvm::Type::getVoidTy(_context->getBackend()->getContext()), {});
    {
        auto symBackSlashNGlobal = std::make_unique<SymboleFunctionGlobale>();
        symBackSlashNGlobal->typeReturn = typeVoidPrysma;
        symBackSlashNGlobal->node = nullptr;
        _context->getRegistryFunctionGlobale()->enregistryr("backSlashN", std::move(symBackSlashNGlobal));

        auto symBackSlashNLocal = std::make_unique<SymboleFunctionLocale>();
        symBackSlashNLocal->function = _context->getBackend()->getModule().getFunction("backSlashN");
        symBackSlashNLocal->typeReturn = typeVoidPrysma;
        symBackSlashNLocal->node = nullptr;
        _context->getRegistryFunctionLocale()->enregistryr("backSlashN", std::move(symBackSlashNLocal));
    }

    // print
    std::vector<llvm::Type*> print_args;
    print_args.push_back(llvm::Type::getInt32Ty(_context->getBackend()->getContext()));
    llvm::FunctionType* print_type = llvm::FunctionType::get(llvm::Type::getVoidTy(_context->getBackend()->getContext()), print_args, true);
    llvm::Function* printFunc = llvm::Function::Create(print_type, llvm::Function::ExternalLinkage, "print", _context->getBackend()->getModule());
    {
        auto symPrintGlobal = std::make_unique<SymboleFunctionGlobale>();
        symPrintGlobal->typeReturn = typeVoidPrysma;
        symPrintGlobal->node = nullptr;
        _context->getRegistryFunctionGlobale()->enregistryr("print", std::move(symPrintGlobal));

        auto symPrintLocal = std::make_unique<SymboleFunctionLocale>();
        symPrintLocal->function = printFunc;
        symPrintLocal->typeReturn = typeVoidPrysma;
        symPrintLocal->node = nullptr;
        _context->getRegistryFunctionLocale()->enregistryr("print", std::move(symPrintLocal));
    }

    // prysma_malloc
    std::vector<llvm::Type*> malloc_args;
    malloc_args.push_back(llvm::Type::getInt64Ty(_context->getBackend()->getContext()));
    llvm::FunctionType* malloc_type = llvm::FunctionType::get(llvm::PointerType::getUnqual(_context->getBackend()->getContext()), malloc_args, false);
    llvm::Function* mallocFunc = llvm::Function::Create(malloc_type, llvm::Function::ExternalLinkage, "prysma_malloc", _context->getBackend()->getModule());
    {
        auto symMallocGlobal = std::make_unique<SymboleFunctionGlobale>();
        symMallocGlobal->typeReturn = static_cast<TypeSimple*>(static_cast<void*>(new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::PointerType::getUnqual(_context->getBackend()->getContext()))));
        symMallocGlobal->node = nullptr;
        _context->getRegistryFunctionGlobale()->enregistryr("prysma_malloc", std::move(symMallocGlobal));

        auto symMallocLocal = std::make_unique<SymboleFunctionLocale>();
        symMallocLocal->function = mallocFunc;
        symMallocLocal->typeReturn = new (_arena.Allocate<TypeSimple>()) TypeSimple(llvm::PointerType::getUnqual(_context->getBackend()->getContext())); // NOLINT(cppcoreguidelines-owning-memory)
        symMallocLocal->node = nullptr;
        _context->getRegistryFunctionLocale()->enregistryr("prysma_malloc", std::move(symMallocLocal));
    }

    // prysma_free
    std::vector<llvm::Type*> free_args;
    free_args.push_back(llvm::PointerType::getUnqual(_context->getBackend()->getContext()));
    llvm::FunctionType* free_type = llvm::FunctionType::get(llvm::Type::getVoidTy(_context->getBackend()->getContext()), free_args, false);
    llvm::Function* freeFunc = llvm::Function::Create(free_type, llvm::Function::ExternalLinkage, "prysma_free", _context->getBackend()->getModule());
    {
        auto symFreeGlobal = std::make_unique<SymboleFunctionGlobale>();
        symFreeGlobal->typeReturn = typeVoidPrysma;
        symFreeGlobal->node = nullptr;
        _context->getRegistryFunctionGlobale()->enregistryr("prysma_free", std::move(symFreeGlobal));

        auto symFreeLocal = std::make_unique<SymboleFunctionLocale>();
        symFreeLocal->function = freeFunc;
        symFreeLocal->typeReturn = typeVoidPrysma;
        symFreeLocal->node = nullptr;
        _context->getRegistryFunctionLocale()->enregistryr("prysma_free", std::move(symFreeLocal));
    }
}

void ConfigurationFacadeEnvironnement::enregistryrTypesDeBase()
{
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_STRING, llvm::Type::getInt8Ty(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_CHAR, llvm::Type::getInt8Ty(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_INT64, llvm::Type::getInt64Ty(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_INT32, llvm::Type::getInt32Ty(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(_context->getBackend()->getContext()));
    _context->getRegistryType()->enregistryr(TOKEN_TYPE_PTR, llvm::PointerType::getUnqual(_context->getBackend()->getContext()));
}

void ConfigurationFacadeEnvironnement::creerContextParser()
{
    if (_builderTreeInstruction == nullptr || _builderEquation == nullptr || _parserType == nullptr) {
        throw std::logic_error("ContextParser ne peut pas être créé avant les builders d'trees et le parser de type");
    }

    ContextParser::Dependencies deps = {
        _builderEquation->recupererBuilderTree(),
        _builderTreeInstruction,
        _parserType,
        _registryVariable.get(),
        _registryType.get()
    };
    _contextParser = new (_arena.Allocate<ContextParser>()) ContextParser(deps); // NOLINT(cppcoreguidelines-owning-memory)
}

void ConfigurationFacadeEnvironnement::enregistryrExpressions()
{
    // Construire les chef d'orchestre de l'tree syntaxique abstrait
    _builderTreeInstruction = new (_arena) // NOLINT(cppcoreguidelines-owning-memory)
        BuilderTreeInstruction(_registryInstruction.get(), _arena); 

    _registryExpression = new (_arena.Allocate<RegistryExpression>()) RegistryExpression(); // NOLINT(cppcoreguidelines-owning-memory)

    _builderEquation = new (_arena) // NOLINT(cppcoreguidelines-owning-memory)
        BuilderEquationFlottante(_registryExpression, _arena);

    // Créer le ParserType avec le registry
    _parserType = new (_arena.Allocate<ParserType>()) // NOLINT(cppcoreguidelines-owning-memory)
        ParserType(_context->getRegistryType(), _builderEquation->recupererBuilderTree());

    if (_contextParser == nullptr) {
        creerContextParser();
    }

    _contexteExpression = new (_arena.Allocate<ContextExpression>()) ContextExpression( // NOLINT(cppcoreguidelines-owning-memory)
        _builderEquation->recupererBuilderTree(),
        _builderTreeInstruction,
        _parserType,
        _contextParser,
        &_arena,
        _registryVariable.get(),
        _registryType.get()
    );

    auto* exprLitInt = new (_arena.Allocate<ExpressionLiteral>()) ExpressionLiteral(*_contexteExpression); // // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_LIT_INT, exprLitInt);

    auto* exprLitFloat = new (_arena.Allocate<ExpressionLiteral>()) ExpressionLiteral(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_LIT_FLOAT, exprLitFloat);

    auto* exprLitBool = new (_arena.Allocate<ExpressionLiteral>()) ExpressionLiteral(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_LIT_BOLEEN, exprLitBool);

    auto* exprIdentifiant = new (_arena.Allocate<ExpressionIdentifiant>()) ExpressionIdentifiant(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_IDENTIFIANT, exprIdentifiant);

    auto* exprRef = new (_arena.Allocate<ExpressionRefVariable>()) ExpressionRefVariable(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_REF, exprRef);

    auto* exprUnRef = new (_arena.Allocate<ExpressionUnRefVariable>()) ExpressionUnRefVariable(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_UNREF, exprUnRef);

    auto* exprNeg = new (_arena.Allocate<ExpressionNegation>()) ExpressionNegation(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_NON, exprNeg);

    auto* exprString = new (_arena.Allocate<ExpressionString>()) ExpressionString(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_GUILLEMET, exprString);

    auto* exprTab = new (_arena.Allocate<ExpressionArrayInitialization>()) ExpressionArrayInitialization(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_CROCHET_OUVERT, exprTab);

    auto* exprCall = new (_arena.Allocate<ExpressionCallCentral>()) ExpressionCallCentral(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_CALL, exprCall);

    auto* exprNew = new (_arena.Allocate<ExpressionNew>()) ExpressionNew(*_contexteExpression); // NOLINT(cppcoreguidelines-owning-memory)
    _registryExpression->enregistryr(TOKEN_NEW, exprNew);
}

void ConfigurationFacadeEnvironnement::enregistryrInstructions()
{
    
    auto* parsFonc = new (_arena.Allocate<ParserDeclarationFunction>()) ParserDeclarationFunction(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_FONCTION, parsFonc);

    auto* parsAff = new (_arena.Allocate<ParserAssignmentVariable>()) ParserAssignmentVariable(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_AFF, parsAff);

    auto* parsDec = new (_arena.Allocate<ParserDeclarationVariable>()) ParserDeclarationVariable(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_DEC, parsDec);

    auto* parsCall = new (_arena.Allocate<ParserCallCentral>()) ParserCallCentral(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_CALL, parsCall);

    auto* parsRet = new (_arena.Allocate<ParserReturn>()) ParserReturn(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_RETOUR, parsRet);

    auto* parsArg = new (_arena.Allocate<ParserArgFunction>()) ParserArgFunction(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_ARG, parsArg);

    auto* parsUnRef = new (_arena.Allocate<ParserUnRefVariable>()) ParserUnRefVariable(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_UNREF, parsUnRef);

    auto* parsRefVar = new (_arena.Allocate<ParserRefVariable>()) ParserRefVariable(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_REF, parsRefVar);

    auto* parsIf = new (_arena.Allocate<ParserIf>()) ParserIf(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_SI, parsIf);

    auto* parsWhile = new (_arena.Allocate<ParserWhile>()) ParserWhile(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_TANT_QUE, parsWhile);

    auto* parsInclude = new (_arena.Allocate<ParserInclude>()) ParserInclude(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_INCLUDE, parsInclude);

    auto* parsDelete = new (_arena.Allocate<ParserDelete>()) ParserDelete(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_DELETE, parsDelete);

    auto* parsClass = new (_arena.Allocate<ParserClass>()) ParserClass(*_contextParser); // NOLINT(cppcoreguidelines-owning-memory)
    _registryInstruction->enregistryr(TOKEN_CLASS, parsClass);
}

auto ConfigurationFacadeEnvironnement::getContext() const -> ContextGenCode*
{
    return _context.get();
}

auto ConfigurationFacadeEnvironnement::getArena() -> llvm::BumpPtrAllocator&
{
    return _arena;
}

auto ConfigurationFacadeEnvironnement::getBuilderTreeInstruction() const -> BuilderTreeInstruction*
{
    return _builderTreeInstruction;
}

auto ConfigurationFacadeEnvironnement::getBuilderEquation() const -> BuilderEquationFlottante*
{
    return _builderEquation;
}
// NOLINTEND(cppcoreguidelines-owning-memory)
