#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <llvm/IR/Instructions.h>
#include <string>

void GeneralVisitorGenCode::visiter(NodeAccesAttribute* nodeAccesAttribute)
{
    std::string nomObject = nodeAccesAttribute->getNomObject().value;
    std::string nomAttribute = nodeAccesAttribute->getNomAttribute().value;

    LoadurVariable loadur(_contextGenCode);
    Symbole objectSymbole = loadur.loadr(nomObject);
    llvm::Value* object = objectSymbole.getAdresse();

    std::string nomClasse = obtenirNomClasseDepuisSymbole(objectSymbole);

    if (nomClasse.empty()) {
        ErrorHelper::errorCompilation("Impossible de déterminer la classe de l'object '" + nomObject + "'");
    }

    auto* classInfo = _contextGenCode->getRegistryClass()->recuperer(nomClasse).get();
    classInfo = ErrorHelper::verifierNonNull(classInfo, "Classe '" + nomClasse + "' non trouvée dans le registry");

    if (!classInfo->getRegistryVariable()->existeVariable(nomAttribute)) {
        ErrorHelper::errorCompilation("Attribute '" + nomAttribute + "' inexistant dans la classe '" + nomClasse + "'");
    }

    auto iterator = classInfo->getMemberIndices().find(nomAttribute);
    if (iterator == classInfo->getMemberIndices().end()) {
        ErrorHelper::errorCompilation("Attribute '" + nomAttribute + "' n'a pas d'index dans " + nomClasse);
    }
    
    unsigned int index = iterator->second;

    auto& builder = _contextGenCode->getBackend()->getBuilder();

    Symbole symboleVar = classInfo->getRegistryVariable()->recupererVariables(nodeAccesAttribute->getNomAttribute());
    
    llvm::Type* typeDuStruct = classInfo->getStructType();

    llvm::Value* attributePtr = builder.CreateStructGEP(typeDuStruct, object, index, nomObject + "_" + nomAttribute + "_ptr");

    llvm::Value* valeurAttribute = builder.CreateLoad(
        symboleVar.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext()), 
        attributePtr, 
        nomObject + "_" + nomAttribute
    );

    _contextGenCode->modifierValeurTemporaire(Symbole(valeurAttribute, symboleVar.getType(), nullptr));
}
