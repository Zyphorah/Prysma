#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/LLVM/GestionFunction.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/Interfaces/IVisitor.h"
#include "Compiler/Visitor/VisitorBaseGenerale.h"
#include "Compiler/Visitor/Extractors/ArgExtractorFunction.h"
#include "Compiler/Utils/PrysmaCast.h"
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

std::unique_ptr<GenerateurDeclarationFunction> GenerateurDeclarationFunction::creer(ContextGenCode* context, NodeDeclarationFunction* node, IVisitor* visitor)
{
    if (!context->getNomClasseCourante().empty()) {
        return std::make_unique<GenerateurDeclarationMethode>(context, node, visitor);
    } 
    return std::make_unique<GenerateurDeclarationStandard>(context, node, visitor);
}

GenerateurDeclarationFunction::GenerateurDeclarationFunction(ContextGenCode* contextGenCode, NodeDeclarationFunction* nodeDeclarationFunction, IVisitor* visitorGeneralCodeGen) 
:   _contextGenCode(contextGenCode),
    _nodeDeclarationFunction(nodeDeclarationFunction),
    _visitorGeneralCodeGen(visitorGeneralCodeGen)
{
}

llvm::Function* GenerateurDeclarationStandard::creerFunction()
{
    std::string nomFunction = getNodeDeclarationFunction()->getNom();
    
    const auto& symbolePtr = getContextGenCode()->getRegistryFunctionLocale()->recuperer(nomFunction);
    if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
        throw std::runtime_error("Error : SymboleFunctionLocale attendu");
    }
    const auto* symbole = prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
    
    llvm::Function* function = symbole->function;

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(getContextGenCode()->getBackend()->getContext(), "entry", function);
    getContextGenCode()->getBackend()->getBuilder().SetInsertPoint(entryBlock);

    return function;
}

llvm::Function* GenerateurDeclarationMethode::creerFunction()
{
    std::string nomFunction = getNodeDeclarationFunction()->getNom();
    std::string nomClasse = getContextGenCode()->getNomClasseCourante();
    auto const& classInfo = getContextGenCode()->getRegistryClass()->recuperer(nomClasse);
    const auto& symbolePtr = classInfo->getRegistryFunctionLocale()->recuperer(nomFunction);
    if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
        throw std::runtime_error("Error : SymboleFunctionLocale attendu");
    }
    const auto* symbole = prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
    
    llvm::Function* function = symbole->function;

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
        
        getContextGenCode()->getRegistryVariable()->enregistryr(thisToken, symboleThis);
        argIndex = 1;
    }

    for (auto* nodeArg : args.arguments) {
        llvm::Argument* arg = function->getArg(static_cast<unsigned int>(argIndex));
        arg->setName(nodeArg->getNom());
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = getContextGenCode()->getBackend()->getBuilder().CreateAlloca(argType);
        getContextGenCode()->getBackend()->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = nodeArg->getNom();
        argumentToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symbole;
        symbole = Symbole(alloca, symbole.getType(), symbole.getTypePointeElement());
        symbole = Symbole(symbole.getAdresse(), nodeArg->getType(), symbole.getTypePointeElement());
        getContextGenCode()->getRegistryVariable()->enregistryr(argumentToken, symbole);
        
        argIndex++;
    }
}

void GenerateurDeclarationStandard::traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args)
{
    size_t argIndex = 0;

    for (auto* nodeArg : args.arguments) {
        llvm::Argument* arg = function->getArg(static_cast<unsigned int>(argIndex));
        arg->setName(nodeArg->getNom());
        
        llvm::Type* argType = arg->getType();
        llvm::AllocaInst* alloca = getContextGenCode()->getBackend()->getBuilder().CreateAlloca(argType);
        getContextGenCode()->getBackend()->getBuilder().CreateStore(arg, alloca);
        
        Token argumentToken;
        argumentToken.value = nodeArg->getNom();
        argumentToken.type = TOKEN_IDENTIFIANT;
        
        Symbole symbole;
        symbole = Symbole(alloca, symbole.getType(), symbole.getTypePointeElement());
        symbole = Symbole(symbole.getAdresse(), nodeArg->getType(), symbole.getTypePointeElement());
        getContextGenCode()->getRegistryVariable()->enregistryr(argumentToken, symbole);
        
        argIndex++;
    }
}

void GenerateurDeclarationFunction::declarerFunction()
{
    llvm::Type* typeDeReturn = getNodeDeclarationFunction()->getTypeReturn()->generatedrTypeLLVM(getContextGenCode()->getBackend()->getContext());
    
    ArgumentsCodeGen argumentsCodeGen;
    if (getNodeDeclarationFunction() != nullptr) {
        for (INode* node : getNodeDeclarationFunction()->getArguments()) {
            ArgExtractorFunction extracteur;
            node->accept(&extracteur);
            if (extracteur.getArg() != nullptr) {
                argumentsCodeGen.arguments.push_back(extracteur.getArg());
                llvm::Type* argType = extracteur.getArg()->getType()->generatedrTypeLLVM(getContextGenCode()->getBackend()->getContext());
                argumentsCodeGen.argTypes.push_back(argType);
            }
        }
    }

    llvm::Function* function = creerFunction();
    
    getContextGenCode()->getReturnContextCompilation()->piler(getNodeDeclarationFunction()->getTypeReturn());
    getContextGenCode()->getRegistryVariable()->piler();
    
    traiterArgumentsConstruit(function, argumentsCodeGen);

    getNodeDeclarationFunction()->getBody()->accept(_visitorGeneralCodeGen);

    llvm::BasicBlock* blocCourant = getContextGenCode()->getBackend()->getBuilder().GetInsertBlock();
    if (blocCourant != nullptr && typeDeReturn->isVoidTy())
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
        if (blocCourant->getTerminator() == nullptr)
        {
            getContextGenCode()->getBackend()->getBuilder().CreateRetVoid();
        }
#pragma GCC diagnostic pop
    }

    getContextGenCode()->getRegistryVariable()->depiler();
    getContextGenCode()->getReturnContextCompilation()->depiler();

    getContextGenCode()->modifierValeurTemporaire(Symbole(function, getContextGenCode()->getValeurTemporaire().getType(), getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
}

// Génération de l'Call

std::unique_ptr<GenerateurCallFunction> GenerateurCallFunction::creer(ContextGenCode* context, IVisitor* visitor)
{
    if (!context->getNomClasseCourante().empty()) {
        return std::make_unique<GenerateurCallMethode>(context, visitor);
    }
    return std::make_unique<GenerateurCallStandard>(context, visitor);
}

GenerateurCallFunction::GenerateurCallFunction(ContextGenCode* context, IVisitor* visitor)
    : _contextGenCode(context), _visitorGeneralCodeGen(visitor)
{
}

const SymboleFunctionLocale* GenerateurCallMethode::obtenirFunctionLocale(const std::string& nomFunction)
{
    std::string nomClasse = getContextGenCode()->getNomClasseCourante();
    auto const& classInfo = getContextGenCode()->getRegistryClass()->recuperer(nomClasse);
    if(classInfo->getRegistryFunctionLocale()->existe(nomFunction)){
        const auto& symbolePtr = classInfo->getRegistryFunctionLocale()->recuperer(nomFunction);
        if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
            throw std::runtime_error("Error : SymboleFunctionLocale attendu");
        }
        return prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
    }
    
    if (getContextGenCode()->getRegistryFunctionLocale()->existe(nomFunction)) {
        const auto& symbolePtr = getContextGenCode()->getRegistryFunctionLocale()->recuperer(nomFunction);
        if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
            throw std::runtime_error("Error : SymboleFunctionLocale attendu");
        }
        return prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
    }

    throw std::runtime_error("Méthode ou function introuvable dans la portée de la classe : " + nomFunction);
}

const SymboleFunctionLocale* GenerateurCallStandard::obtenirFunctionLocale(const std::string& nomFunction)
{
    if (getContextGenCode()->getRegistryFunctionLocale()->existe(nomFunction)) {
        const auto& symbolePtr = getContextGenCode()->getRegistryFunctionLocale()->recuperer(nomFunction);
        if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
            throw std::runtime_error("Error : SymboleFunctionLocale attendu");
        }
        return prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
    }

    throw std::runtime_error("Function introuvable dans le registry local : " + nomFunction);
}

void GenerateurCallFunction::generatedrCallFunction(NodeCallFunction* nodeCallFunction)
{
    std::string nomFunction = nodeCallFunction->getNomFunction().value;

    if (RegistryBuiltIns::estBuiltIn(nomFunction)) {
        RegistryBuiltIns::generatedrCall(nomFunction, nodeCallFunction, getContextGenCode(), _visitorGeneralCodeGen);
        return; 
    }

    getContextGenCode()->getRegistryArgument()->vider();

    const SymboleFunctionLocale* symboleFunction = obtenirFunctionLocale(nomFunction);
    llvm::Function* functionCible = symboleFunction->function;
    llvm::FunctionType* typeFunction = functionCible->getFunctionType();
    
    unsigned int indexParam = 0; 
    for (INode* argumentChild : nodeCallFunction->getChilds()) 
    {
        argumentChild->accept(_visitorGeneralCodeGen);
        llvm::Value* valeurArgument = getContextGenCode()->getValeurTemporaire().getAdresse();

        if (valeurArgument == nullptr) {
            throw std::runtime_error("Error : L'argument n'a pas généré de valeur.");
        }

        llvm::Value* valeurFinale = valeurArgument;
        if (indexParam < typeFunction->getNumParams()) {
            llvm::Type* typeAttendu = typeFunction->getParamType(indexParam);
            valeurFinale = getContextGenCode()->getBackend()->creerAutoCast(valeurArgument, typeAttendu);
        }

        getContextGenCode()->getRegistryArgument()->ajouter(valeurFinale);
        indexParam++;
    }

    std::vector<llvm::Value*> args = getContextGenCode()->getRegistryArgument()->recuperer();
    getContextGenCode()->getRegistryArgument()->vider();

    if(functionCible->getReturnType()->isVoidTy())
    {
        getContextGenCode()->getBackend()->getBuilder().CreateCall(functionCible, args);
        getContextGenCode()->modifierValeurTemporaire(Symbole(nullptr, getContextGenCode()->getValeurTemporaire().getType(), getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
        getContextGenCode()->modifierValeurTemporaire(Symbole(getContextGenCode()->getValeurTemporaire().getAdresse(), nullptr, getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
        return;
    }
    
    getContextGenCode()->modifierValeurTemporaire(Symbole(getContextGenCode()->getBackend()->getBuilder().CreateCall(
        functionCible, 
        args, 
        "resultat_call"
    ), getContextGenCode()->getValeurTemporaire().getType(), getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
    getContextGenCode()->modifierValeurTemporaire(Symbole(getContextGenCode()->getValeurTemporaire().getAdresse(), symboleFunction->typeReturn, getContextGenCode()->getValeurTemporaire().getTypePointeElement()));
}


// Gestion des functions Natives (Built-ins)

bool RegistryBuiltIns::estBuiltIn(const std::string& nom) {
    return nom == "print";
}

void RegistryBuiltIns::generatedrCall(const std::string& nom, NodeCallFunction* nodeCallFunction, ContextGenCode* context, IVisitor* visitor) {
    if (nom == "print") {
        if (nodeCallFunction->getChilds().empty()) {
            return;
        }

        nodeCallFunction->getChilds()[0]->accept(visitor);
        llvm::Value* valeurArgument = context->getValeurTemporaire().getAdresse();
        IType* typeArgument = context->getValeurTemporaire().getType();
        
        if (valeurArgument == nullptr) {
            throw std::runtime_error("Error : L'argument de print n'a pas généré de valeur.");
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
        
        const auto& symbolePtr = context->getRegistryFunctionLocale()->recuperer("print");
        if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
            throw std::runtime_error("Error : SymboleFunctionLocale attendu pour 'print'");
        }
        const auto* symbolePrint = prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
        
        builder.CreateCall(symbolePrint->function, { llvmTag, valeurArgument });
        context->modifierValeurTemporaire(Symbole(nullptr, context->getValeurTemporaire().getType(), context->getValeurTemporaire().getTypePointeElement()));
    }
}
