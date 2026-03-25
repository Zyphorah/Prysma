#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/Utils/PrysmaCast.h"
#include "Compiler/LLVM/LlvmBackend.h"

#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <memory>
#include <string>
#include <utility>

// Passe 1 : Enregistryment des types opaques de classes
// On crée un StructType opaque (sans body) pour chaque classe déclarée.
// Cela permet de résoudre les dépendances circulaires input classes
// lors de la Passe 2 (remplissage du body des struct).

void FillingVisitorRegistry::visiter(NodeClass* nodeClass)
{
    // 1. Récupérer le nom de la classe à partir du nœud de déclaration
    const Token& nomClasseToken = nodeClass->getNomClass();
    std::string nomClasse = nomClasseToken.value;

    // 2. Créer le type "Opaque" dans LLVM
    llvm::StructType* typeOpaqueLLVM = llvm::StructType::create(
        _contextGenCode->getBackend()->getContext(),
        "Class_" + nomClasse
    );

    // 3. Créer l'instance de la structure "Class" qui contient les métadonnées
    auto infosClasse = std::make_unique<Class>();

    // 4. Lier le type LLVM opaque à la structure
    infosClasse->setStructType(typeOpaqueLLVM);

    // 5. Initialiser l'héritage
    const auto& heritage = nodeClass->getHeritage();
    if (!heritage.empty()) {
        infosClasse->setParentHeritage(heritage[0]);
    } else {
        infosClasse->setParentHeritage(nullptr);
    }

    // 6. Préparer les registrys internes (vides pour l'instant, seront remplis en Passe 2)
    infosClasse->setRegistryVariable(new RegistryVariable());
    infosClasse->setRegistryFunctionLocale(new RegistryFunctionLocale());

    // La VTable sera générée plus tard lors de la résolution des méthodes virtuelles
    infosClasse->setVTable(nullptr);

    Class* infosClassePtr = infosClasse.get();

    // 7. Enregistryr la classe dans le registry global du compiler
    _contextGenCode->getRegistryClass()->enregistryr(nomClasse, std::move(infosClasse));

    // 8. Visiter le body de la classe pour remplir ses registrys (méthodes, etc.)
    std::string ancienneClasse = _contextGenCode->getNomClasseCourante();
    _contextGenCode->modifierNomClasseCourante(nomClasse);

    for (auto* membre : nodeClass->getListMembers()) {
        if (prysma::isa<NodeDeclarationFunction>(membre)) {
            membre->accept(this);
        }
        else if (auto* declVar = prysma::dyn_cast<NodeDeclarationVariable>(membre)) {
            Token token;
            token.value = declVar->getNom();
            infosClassePtr->getRegistryVariable()->enregistryr(token, Symbole(nullptr, declVar->getType()));
            
            if (declVar->getExpression() != nullptr) {
                infosClassePtr->getMemberInitializers()[declVar->getNom()] = declVar->getExpression();
            }
        }
    }

    // Aussi visiter les builders si nécessaire :
    for (auto* builder : nodeClass->getBuilder()) {
        builder->accept(this);
    }

    _contextGenCode->modifierNomClasseCourante(ancienneClasse);
}
