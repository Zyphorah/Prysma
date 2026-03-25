#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include "Compiler/AST/Registry/Types/TypeArray.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/Utils/PrysmaCast.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <cstddef>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/Support/Casting.h>
#include <string>

void GeneralVisitorGenCode::visiter(NodeReadingArray* nodeReadingArray)
{
    std::string nomArrayStr = nodeReadingArray->getNomArray().value;
    Symbole symbole;
    llvm::Value* adresseArray = nullptr;

    if (nomArrayStr.find('.') != std::string::npos) {
        size_t pos = nomArrayStr.find('.');
        std::string nomObject = nomArrayStr.substr(0, pos);
        std::string nomAttribute = nomArrayStr.substr(pos + 1);

        LoadurVariable loadur(_contextGenCode);
        Symbole objectSymbole = loadur.loadr(nomObject);
        llvm::Value* object = objectSymbole.getAdresse();

        std::string nomClasse = obtenirNomClasseDepuisSymbole(objectSymbole);
        auto* classInfo = _contextGenCode->getRegistryClass()->recuperer(nomClasse).get();
        auto iterator = classInfo->getMemberIndices().find(nomAttribute);
        unsigned int indexObj = iterator->second;

        Token tokenAttribute; tokenAttribute.value = nomAttribute; tokenAttribute.type = TOKEN_IDENTIFIANT;
        symbole = classInfo->getRegistryVariable()->recupererVariables(tokenAttribute);
        
        llvm::Type* typeDuStruct = classInfo->getStructType();
        adresseArray = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(typeDuStruct, object, indexObj, nomObject + "_" + nomAttribute + "_ptr");
    } else {
        AdresseurVariable adresseur(_contextGenCode);
        symbole = adresseur.recupererAdresse(nomArrayStr);
        adresseArray = symbole.getAdresse();
    }

    llvm::Type* typeArray = nullptr;
    if (symbole.getType() != nullptr) {
        typeArray = symbole.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
    } else {
        auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseArray);
        allocaInst = ErrorHelper::verifierNonNull(allocaInst, "L'adresse du array n'est pas une instruction d'allocation");
        typeArray = allocaInst->getAllocatedType();
    }
    
    // Calculer l'index à partir de l'équation 
    nodeReadingArray->getIndexEquation()->accept(this);
    llvm::Value* indexValue = _contextGenCode->getValeurTemporaire().getAdresse();
    
    llvm::Value* adresseElement = nullptr;
    llvm::Type* typeElement = nullptr;

    if (typeArray->isArrayTy()) {
        // calculer le décalage avec GEP 
        adresseElement = _contextGenCode->getBackend()->getBuilder().CreateGEP(typeArray, adresseArray, 
            { 
                _contextGenCode->getBackend()->getBuilder().getInt32(0), 
                indexValue 
            }, nodeReadingArray->getNomArray().value);
        typeElement = typeArray->getArrayElementType();
    } else {
        // C'est un pointeur vers un array (référence), on doit loadr le pointeur
        llvm::Value* loadedPtr = _contextGenCode->getBackend()->getBuilder().CreateLoad(typeArray, adresseArray);
        
        IType* astType = symbole.getType();
        auto* typeArrayDecla = prysma::dyn_cast<TypeArray>(astType);
        if (typeArrayDecla != nullptr) {
            typeElement = typeArrayDecla->getTypeChild()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        } else {
            typeElement = symbole.getTypePointeElement();
        }
        
        adresseElement = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            typeElement, 
            loadedPtr, 
            indexValue,
            nodeReadingArray->getNomArray().value
        );
    }
        
    // Lire la valeur de l'adresse de l'élément
    llvm::Value* valeurElement = _contextGenCode->getBackend()->getBuilder().CreateLoad(typeElement, adresseElement, nodeReadingArray->getNomArray().value);

    _contextGenCode->modifierValeurTemporaire(Symbole(valeurElement, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(typeElement), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
} 

