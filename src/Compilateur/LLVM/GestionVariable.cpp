#include "Compilateur/LLVM/GestionVariable.h"
#include <llvm-18/llvm/IR/Type.h>
#include <stdexcept>

GestionVariable::GestionVariable(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{
}

GestionVariable::~GestionVariable()
{
}

llvm::Type* GestionVariable::extraireTypeDonnee(llvm::Value* adresseMemoire)
{
    llvm::Type* typeDonnee = nullptr;

    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        typeDonnee = allocaInst->getAllocatedType();
    } else {
        typeDonnee = llvm::Type::getFloatTy(_contextGenCode->backend->getContext());
    }

    return typeDonnee;
}

Symbole GestionVariable::chargerVariable(const std::string& nomVariable)
{
    Symbole symbole = recupererAdresseVariable(nomVariable);
    llvm::Value* adresseMemoire = symbole.adresse;
    
    llvm::Type* typeDonneeVal = nullptr;
    if (symbole.type != nullptr) {
        typeDonneeVal = symbole.type->genererTypeLLVM(_contextGenCode->backend->getContext());
    } else {
        typeDonneeVal = extraireTypeDonnee(adresseMemoire);
    }
    
    // Un seul saut : charger la valeur stockée à l'adresse mémoire
    llvm::Value* valeurChargee = _contextGenCode->backend->getBuilder().CreateLoad(
        typeDonneeVal,
        adresseMemoire,
        nomVariable
    );

    symbole.adresse = valeurChargee;
    
    return symbole;
}

Symbole GestionVariable::chargerVariableUnref(const std::string& nomVariable)
{
    Symbole symbole = recupererAdresseVariable(nomVariable);
    llvm::Value* adresseMemoire = symbole.adresse;
    
    llvm::Type* typeDonneeVal = nullptr;
    if (symbole.type != nullptr) {
        typeDonneeVal = symbole.type->genererTypeLLVM(_contextGenCode->backend->getContext());
    } else {
        typeDonneeVal = extraireTypeDonnee(adresseMemoire);
    }
    
    if (adresseMemoire == nullptr) 
    {
        throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
    }

    if(typeDonneeVal->isPointerTy())
    {   
        // Premier saut : charger la valeur à l'adresse mémoire (le pointeur)
        llvm::Value* adressePointeur = _contextGenCode->backend->getBuilder().CreateLoad(
            typeDonneeVal,
            adresseMemoire,
            nomVariable
        );

        // Récupérer le type de ce qui est pointé
        llvm::Type* typePointeElement = symbole.typePointeElement;
        if (typePointeElement == nullptr) {
            typePointeElement = llvm::Type::getInt32Ty(_contextGenCode->backend->getContext());
        }

        // Deuxième saut : charger la valeur à l'adresse du pointeur avec le bon type
        llvm::Value* valeurChargeDeuxiemeSaut = _contextGenCode->backend->getBuilder().CreateLoad(
            typePointeElement,
            adressePointeur,
            nomVariable + "_unref"
        );

        symbole.adresse = valeurChargeDeuxiemeSaut;
    }
    else {
        throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas un pointeur. Unref ne peut s'appliquer qu'à des pointeurs.");
    }

    return symbole;
}

llvm::AllocaInst* GestionVariable::allouerVariable(llvm::Type* type, const std::string& nomVariable)
{
    llvm::BasicBlock* insertBlock = _contextGenCode->backend->getBuilder().GetInsertBlock();
    _contextGenCode->backend->definirPointInsertionApresAllocation();

    llvm::AllocaInst* allocaInst = _contextGenCode->backend->getBuilder().CreateAlloca(type, nullptr, nomVariable);
    
    _contextGenCode->backend->getBuilder().SetInsertPoint(insertBlock);

    return allocaInst;
}

void GestionVariable::stockerVariable(llvm::Value* valeur, llvm::AllocaInst* allocaInst)
{
    _contextGenCode->backend->getBuilder().CreateStore(valeur, allocaInst);
}

void GestionVariable::affecterVariable(llvm::Value* variableExistante, llvm::Value* valeur, llvm::Type* typeVariableLLVM)
{
    llvm::Type* typeAllouer = typeVariableLLVM;
    if (typeAllouer == nullptr) {
        if (auto* allocaValue = llvm::dyn_cast<llvm::AllocaInst>(variableExistante)) {
            typeAllouer = allocaValue->getAllocatedType();
        } else {
            typeAllouer = extraireTypeDonnee(variableExistante);
        }
    }

    // Si c'est un pointeur alors nous devons accéder à la valeur stockée au pointeur sinon la valeur se trouve directement dans l'allocaInst
    if(typeAllouer->isPointerTy())
    {
        llvm::Value* valeurCharger = _contextGenCode->backend->getBuilder().CreateLoad(
            _contextGenCode->backend->getBuilder().getPtrTy(),
            variableExistante,
            "ptr_chargé"
        );
        _contextGenCode->backend->getBuilder().CreateStore(valeur, valeurCharger);
    }
    else
    {
        // Pour les variables non-pointeur, on peut caster si nécessaire
        llvm::Value* valeurCast = _contextGenCode->backend->creerAutoCast(valeur, typeAllouer);
        _contextGenCode->backend->getBuilder().CreateStore(valeurCast, variableExistante);
    }
}

Symbole GestionVariable::recupererAdresseVariable(const std::string& nomVariable)
{
    Token tokenRecherche;
    tokenRecherche.value = nomVariable;

    if (_contextGenCode->registreVariable->existeVariable(nomVariable)) {
        return _contextGenCode->registreVariable->recupererVariables(tokenRecherche);
    }

    if (!_contextGenCode->nomClasseCourante.empty()) {
        auto* classInfo = _contextGenCode->registreClass->recuperer(_contextGenCode->nomClasseCourante);
        if (classInfo->memberIndices.find(nomVariable) != classInfo->memberIndices.end()) {
            unsigned int idx = classInfo->memberIndices[nomVariable];
            
            // Récupérer le symbole modèle du registre class (si dispo)
            Symbole modeleSymbole;
            if(classInfo->registreVariable->existeVariable(nomVariable)) {
                modeleSymbole = classInfo->registreVariable->recupererVariables(tokenRecherche);
            }

            Token thisToken; thisToken.value = "this";
            Symbole thisSymbole = _contextGenCode->registreVariable->recupererVariables(thisToken);
            
            llvm::Value* thisPtrAddr = thisSymbole.adresse;
            llvm::Type* pointeurType = llvm::PointerType::getUnqual(_contextGenCode->backend->getContext());

            llvm::Value* thisPtr = _contextGenCode->backend->getBuilder().CreateLoad(pointeurType, thisPtrAddr, "this_val");

            std::vector<llvm::Value*> indices = {
                _contextGenCode->backend->getBuilder().getInt32(0),
                _contextGenCode->backend->getBuilder().getInt32(idx)
            };

            llvm::Value* fieldPtr = _contextGenCode->backend->getBuilder().CreateGEP(
                classInfo->structType,
                thisPtr,
                indices,
                nomVariable + "_ptr"
            );

            return Symbole(fieldPtr, modeleSymbole.type, modeleSymbole.typePointeElement);
        }
    }

    throw std::runtime_error("Erreur sémantique : La variable '" + nomVariable + "' n'est pas déclarée.");
}
