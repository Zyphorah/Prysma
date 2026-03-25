#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/TypeArray.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <cstddef>
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <string>
#include <vector>

void GeneralVisitorGenCode::visiter(NodeAssignmentArray* nodeAssignmentArray)
{
    // Évaluer l'expression de l'index
    llvm::Value* indexValue = evaluerExpression(nodeAssignmentArray->getExpressionIndex()).getAdresse();
  
    // Évaluer l'expression à affecter
    llvm::Value* expressionResult = evaluerExpression(nodeAssignmentArray->getExpression()).getAdresse();

    // Récupérer le array
    std::string nomArrayStr = nodeAssignmentArray->getToken().value;
    Symbole symbole;
    llvm::Value* valeur = nullptr;

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
        valeur = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(typeDuStruct, object, indexObj, nomObject + "_" + nomAttribute + "_ptr");
    } else {
        AdresseurVariable adresseur(_contextGenCode);
        symbole = adresseur.recupererAdresse(nomArrayStr);
        valeur = symbole.getAdresse();
    }

    // Assignment à un élément du array avec l'index
    std::vector<llvm::Value*> indices = { 
        _contextGenCode->getBackend()->getBuilder().getInt32(0), 
        indexValue 
    };
    
    // On ne peut pas récupérer la taille du array à la compilation si l'index est une variable.
    // On doit donc récupérer le type du array directement depuis l'instruction d'allocation.
    llvm::Type* arrayType = nullptr;
    if (symbole.getType() != nullptr) {
        arrayType = symbole.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
    } else {
        auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(valeur);
        if (allocaInst != nullptr) {
            arrayType = allocaInst->getAllocatedType();
        }
    }

    llvm::Value* ptrCase = nullptr;
    llvm::Type* typeElement = nullptr;

    if (arrayType->isArrayTy()) {
        ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            arrayType, 
            valeur, 
            indices
        );
        typeElement = arrayType->getArrayElementType();
    } else {
        // C'est un pointeur vers un array (référence), on doit loadr le pointeur
        llvm::Value* loadedPtr = _contextGenCode->getBackend()->getBuilder().CreateLoad(arrayType, valeur);
        
        IType* astType = symbole.getType();
        auto* typeArrayDecla = prysma::dyn_cast<TypeArray>(astType);
        if (typeArrayDecla != nullptr) {
            typeElement = typeArrayDecla->getTypeChild()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        } else {
            typeElement = symbole.getTypePointeElement();
        }
        
        ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            typeElement, 
            loadedPtr, 
            indexValue
        );
    }

    llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(expressionResult, typeElement);
    
    _contextGenCode->getBackend()->getBuilder().CreateStore(valeurCastee, ptrCase);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}