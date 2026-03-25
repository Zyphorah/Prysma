#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <memory>
#include <string>
#include <utility>

void FillingVisitorRegistry::visiter(NodeDeclarationFunction* nodeDeclarationFunction)
{
    IType* typeReturn = nodeDeclarationFunction->getTypeReturn();
    std::string nomFunction = nodeDeclarationFunction->getNom();
    
    if (_contextGenCode->getNomClasseCourante() != "") {
        // contexte de la class (méthode)
        auto symboleFunction = std::make_unique<SymboleFunctionLocale>();
        symboleFunction->typeReturn = typeReturn;
        symboleFunction->node = nodeDeclarationFunction;

        std::string nomClasse = _contextGenCode->getNomClasseCourante();
        auto* classInfo = _contextGenCode->getRegistryClass()->recuperer(nomClasse).get();
        classInfo->getRegistryFunctionLocale()->enregistryr(nomFunction, std::move(symboleFunction));
    } else {
        // contexte global (function globale)
        auto symboleFunction = std::make_unique<SymboleFunctionGlobale>();
        symboleFunction->typeReturn = typeReturn;
        symboleFunction->node = nodeDeclarationFunction;

        _contextGenCode->getRegistryFunctionGlobale()->enregistryr(nomFunction, std::move(symboleFunction));
    }
}