#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <string>
#include <vector>

ExtracteurTypeVariable::ExtracteurTypeVariable(ContextGenCode* context) : _context(context) {}

llvm::Type* ExtracteurTypeVariable::extraire(llvm::Value* adresseMemoire) {
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        return allocaInst->getAllocatedType();
    }
    return llvm::Type::getFloatTy(_context->getBackend()->getContext());
}

AdresseurVariable::AdresseurVariable(ContextGenCode* context) : _context(context) {}

Symbole AdresseurVariable::recupererAdresse(const std::string& nomVariable) {
    Token tokenRecherche;
    tokenRecherche.value = nomVariable;

    if (_context->getRegistreVariable()->existeVariable(nomVariable)) {
        return _context->getRegistreVariable()->recupererVariables(tokenRecherche);
    }

    if (!_context->getNomClasseCourante().empty()) {
        auto const& classInfo = _context->getRegistreClass()->recuperer(_context->getNomClasseCourante());
        if (classInfo->getMemberIndices().find(nomVariable) != classInfo->getMemberIndices().end()) {
            unsigned int idx = classInfo->getMemberIndices().at(nomVariable);
            
            Symbole modeleSymbole;
            if(classInfo->getRegistreVariable()->existeVariable(nomVariable)) {
                modeleSymbole = classInfo->getRegistreVariable()->recupererVariables(tokenRecherche);
            }

            Token thisToken; thisToken.value = "this";
            Symbole thisSymbole = _context->getRegistreVariable()->recupererVariables(thisToken);
            
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

    throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
}

ChargeurVariable::ChargeurVariable(ContextGenCode* context) 
    : _context(context), _adresseur(context), _extracteur(context) {}

Symbole ChargeurVariable::charger(const std::string& nomVariable) {
    Symbole symbole = _adresseur.recupererAdresse(nomVariable);
    llvm::Value* adresseMemoire = symbole.getAdresse();
    
    llvm::Type* typeDonneeVal = nullptr;
    if (symbole.getType() != nullptr) {
        typeDonneeVal = symbole.getType()->genererTypeLLVM(_context->getBackend()->getContext());
    } else {
        typeDonneeVal = _extracteur.extraire(adresseMemoire);
    }
    
    llvm::Value* valeurChargee = _context->getBackend()->getBuilder().CreateLoad(
        typeDonneeVal,
        adresseMemoire,
        nomVariable
    );

    symbole = Symbole(valeurChargee, symbole.getType(), symbole.getTypePointeElement());
    return symbole;
}

Symbole ChargeurVariable::chargerUnref(const std::string& nomVariable) {
    Symbole symbole = _adresseur.recupererAdresse(nomVariable);
    llvm::Value* adresseMemoire = symbole.getAdresse();
    
    llvm::Type* typeDonneeVal = nullptr;
    if (symbole.getType() != nullptr) {
        typeDonneeVal = symbole.getType()->genererTypeLLVM(_context->getBackend()->getContext());
    } else {
        typeDonneeVal = _extracteur.extraire(adresseMemoire);
    }
    
    if (adresseMemoire == nullptr) {
        throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
    }

    if(typeDonneeVal->isPointerTy()) {   
        llvm::Value* adressePointeur = _context->getBackend()->getBuilder().CreateLoad(
            typeDonneeVal,
            adresseMemoire,
            nomVariable
        );

        llvm::Type* typePointeElement = symbole.getTypePointeElement();
        if (typePointeElement == nullptr) {
            typePointeElement = llvm::Type::getInt32Ty(_context->getBackend()->getContext());
        }

        llvm::Value* valeurChargeDeuxiemeSaut = _context->getBackend()->getBuilder().CreateLoad(
            typePointeElement,
            adressePointeur,
            nomVariable + "_unref"
        );

        symbole = Symbole(valeurChargeDeuxiemeSaut, symbole.getType(), symbole.getTypePointeElement());
    } else {
        throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas un pointeur. Unref ne peut s'appliquer qu'à des pointeurs.");
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
        llvm::Value* valeurCharger = _context->getBackend()->getBuilder().CreateLoad(
            _context->getBackend()->getBuilder().getPtrTy(),
            variableExistante,
            "ptr_chargé"
        );
        _context->getBackend()->getBuilder().CreateStore(valeur, valeurCharger);
    } else {
        llvm::Value* valeurCast = _context->getBackend()->creerAutoCast(valeur, typeAllouer);
        _context->getBackend()->getBuilder().CreateStore(valeurCast, variableExistante);
    }
}
