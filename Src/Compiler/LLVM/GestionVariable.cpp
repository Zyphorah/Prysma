#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <string>
#include <vector>

ExtractorTypeVariable::ExtractorTypeVariable(ContextGenCode* context) : _context(context) {}

llvm::Type* ExtractorTypeVariable::extraire(llvm::Value* adresseMemory) {
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemory)) {
        return allocaInst->getAllocatedType();
    }
    return llvm::Type::getFloatTy(_context->getBackend()->getContext());
}

AdresseurVariable::AdresseurVariable(ContextGenCode* context) : _context(context) {}

Symbole AdresseurVariable::recupererAdresse(const std::string& nomVariable) {
    Token tokenRecherche;
    tokenRecherche.value = nomVariable;

    if (_context->getRegistryVariable()->existeVariable(nomVariable)) {
        return _context->getRegistryVariable()->recupererVariables(tokenRecherche);
    }

    if (!_context->getNomClasseCourante().empty()) {
        auto const& classInfo = _context->getRegistryClass()->recuperer(_context->getNomClasseCourante());
        if (classInfo->getMemberIndices().find(nomVariable) != classInfo->getMemberIndices().end()) {
            unsigned int idx = classInfo->getMemberIndices().at(nomVariable);
            
            Symbole modeleSymbole;
            if(classInfo->getRegistryVariable()->existeVariable(nomVariable)) {
                modeleSymbole = classInfo->getRegistryVariable()->recupererVariables(tokenRecherche);
            }

            Token thisToken; thisToken.value = "this";
            Symbole thisSymbole = _context->getRegistryVariable()->recupererVariables(thisToken);
            
            llvm::Value* thisPtrAddr = thisSymbole.getAdresse();
            llvm::Type* pointeurType = llvm::PointerType::getUnqual(_context->getBackend()->getContext());

            llvm::Value* thisPtr = _context->getBackend()->getBuilder().CreateLoad(pointeurType, thisPtrAddr, "this_val");

            std::vector<llvm::Value*> indices = {
                _context->getBackend()->getBuilder().getInt32(0),
                _context->getBackend()->getBuilder().getInt32(idx)
            };

            llvm::Value* fieldPtr = _context->getBackend()->getBuilder().CreateGEP(
                classInfo->getStructType(),
                thisPtr,
                indices,
                nomVariable + "_ptr"
            );

            return Symbole(fieldPtr, modeleSymbole.getType(), modeleSymbole.getTypePointeElement());
        }
    }

    throw std::runtime_error("Error sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
}

LoadurVariable::LoadurVariable(ContextGenCode* context) 
    : _context(context), _adresseur(context), _extracteur(context) {}

Symbole LoadurVariable::loadr(const std::string& nomVariable) {
    Symbole symbole = _adresseur.recupererAdresse(nomVariable);
    llvm::Value* adresseMemory = symbole.getAdresse();
    
    llvm::Type* typeDonneeVal = nullptr;
    if (symbole.getType() != nullptr) {
        typeDonneeVal = symbole.getType()->generatedrTypeLLVM(_context->getBackend()->getContext());
    } else {
        typeDonneeVal = _extracteur.extraire(adresseMemory);
    }
    
    llvm::Value* valeurLoade = _context->getBackend()->getBuilder().CreateLoad(
        typeDonneeVal,
        adresseMemory,
        nomVariable
    );

    symbole = Symbole(valeurLoade, symbole.getType(), symbole.getTypePointeElement());
    return symbole;
}

Symbole LoadurVariable::loadrUnref(const std::string& nomVariable) {
    Symbole symbole = _adresseur.recupererAdresse(nomVariable);
    llvm::Value* adresseMemory = symbole.getAdresse();
    
    llvm::Type* typeDonneeVal = nullptr;
    if (symbole.getType() != nullptr) {
        typeDonneeVal = symbole.getType()->generatedrTypeLLVM(_context->getBackend()->getContext());
    } else {
        typeDonneeVal = _extracteur.extraire(adresseMemory);
    }
    
    if (adresseMemory == nullptr) {
        throw std::runtime_error("Error sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
    }

    if(typeDonneeVal->isPointerTy()) {   
        llvm::Value* adressePointer = _context->getBackend()->getBuilder().CreateLoad(
            typeDonneeVal,
            adresseMemory,
            nomVariable
        );

        llvm::Type* typePointeElement = symbole.getTypePointeElement();
        if (typePointeElement == nullptr) {
            typePointeElement = llvm::Type::getInt32Ty(_context->getBackend()->getContext());
        }

        llvm::Value* valeurLoadDeuxiemeSaut = _context->getBackend()->getBuilder().CreateLoad(
            typePointeElement,
            adressePointer,
            nomVariable + "_unref"
        );

        symbole = Symbole(valeurLoadDeuxiemeSaut, symbole.getType(), symbole.getTypePointeElement());
    } else {
        throw std::runtime_error("Error sémantique : La variable '" + nomVariable + "' n'est pas un pointeur. Unref ne peut s'appliquer qu'à des pointeurs.");
    }

    return symbole;
}

AllocateurVariable::AllocateurVariable(ContextGenCode* context) : _context(context) {}

llvm::AllocaInst* AllocateurVariable::allouer(llvm::Type* type, const std::string& nomVariable) {
    llvm::BasicBlock* insertBlock = _context->getBackend()->getBuilder().GetInsertBlock();
    _context->getBackend()->definirPointInsertionApresAllocation();

    llvm::AllocaInst* allocaInst = _context->getBackend()->getBuilder().CreateAlloca(type, nullptr, nomVariable);
    
    _context->getBackend()->getBuilder().SetInsertPoint(insertBlock);

    return allocaInst;
}

void AllocateurVariable::stocker(llvm::Value* valeur, llvm::AllocaInst* allocaInst) {
    _context->getBackend()->getBuilder().CreateStore(valeur, allocaInst);
}

AffecteurVariable::AffecteurVariable(ContextGenCode* context) 
    : _context(context), _extracteur(context) {}

void AffecteurVariable::affecter(llvm::Value* variableExistante, llvm::Value* valeur, llvm::Type* typeVariableLLVM) {
    llvm::Type* typeAllouer = typeVariableLLVM;
    if (typeAllouer == nullptr) {
        if (auto* allocaValue = llvm::dyn_cast<llvm::AllocaInst>(variableExistante)) {
            typeAllouer = allocaValue->getAllocatedType();
        } else {
            typeAllouer = _extracteur.extraire(variableExistante);
        }
    }

    if(typeAllouer->isPointerTy()) {
        llvm::Value* valeurLoadr = _context->getBackend()->getBuilder().CreateLoad(
            _context->getBackend()->getBuilder().getPtrTy(),
            variableExistante,
            "ptr_chargé"
        );
        _context->getBackend()->getBuilder().CreateStore(valeur, valeurLoadr);
    } else {
        llvm::Value* valeurCast = _context->getBackend()->creerAutoCast(valeur, typeAllouer);
        _context->getBackend()->getBuilder().CreateStore(valeurCast, variableExistante);
    }
}
