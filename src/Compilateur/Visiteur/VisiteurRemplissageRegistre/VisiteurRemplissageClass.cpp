#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include "Compilateur/LLVM/LlvmBackend.h"

#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <memory>
#include <string>
#include <utility>

// Passe 1 : Enregistrement des types opaques de classes
// On crée un StructType opaque (sans corps) pour chaque classe déclarée.
// Cela permet de résoudre les dépendances circulaires entre classes
// lors de la Passe 2 (remplissage du corps des struct).

void VisiteurRemplissageRegistre::visiter(NoeudClass* noeudClass)
{
    // 1. Récupérer le nom de la classe à partir du nœud de déclaration
    const Token& nomClasseToken = noeudClass->getNomClass();
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
    const auto& heritage = noeudClass->getHeritage();
    if (!heritage.empty()) {
        infosClasse->setParentHeritage(heritage[0]);
    } else {
        infosClasse->setParentHeritage(nullptr);
    }

    // 6. Préparer les registres internes (vides pour l'instant, seront remplis en Passe 2)
    infosClasse->setRegistreVariable(new RegistreVariable());
    infosClasse->setRegistreFonctionLocale(new RegistreFonctionLocale());

    // La VTable sera générée plus tard lors de la résolution des méthodes virtuelles
    infosClasse->setVTable(nullptr);

    Class* infosClassePtr = infosClasse.get();

    // 7. Enregistrer la classe dans le registre global du compilateur
    _contextGenCode->getRegistreClass()->enregistrer(nomClasse, std::move(infosClasse));

    // 8. Visiter le corps de la classe pour remplir ses registres (méthodes, etc.)
    std::string ancienneClasse = _contextGenCode->getNomClasseCourante();
    _contextGenCode->modifierNomClasseCourante(nomClasse);

    for (auto* membre : noeudClass->getListMembres()) {
        if (prysma::isa<NoeudDeclarationFonction>(membre)) {
            membre->accept(this);
        }
        else if (auto* declVar = prysma::dyn_cast<NoeudDeclarationVariable>(membre)) {
            Token token;
            token.value = declVar->getNom();
            infosClassePtr->getRegistreVariable()->enregistrer(token, Symbole(nullptr, declVar->getType()));
            
            if (declVar->getExpression() != nullptr) {
                infosClassePtr->getMemberInitializers()[declVar->getNom()] = declVar->getExpression();
            }
        }
    }

    // Aussi visiter les constructeurs si nécessaire :
    for (auto* constructeur : noeudClass->getConstructeur()) {
        constructeur->accept(this);
    }

    _contextGenCode->modifierNomClasseCourante(ancienneClasse);
}
