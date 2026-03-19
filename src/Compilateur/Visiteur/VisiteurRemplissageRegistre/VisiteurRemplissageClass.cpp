#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/LLVM/LlvmBackend.h"

#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <string>

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
        _contextGenCode->backend->getContext(),
        "Class_" + nomClasse
    );

    // 3. Créer l'instance de la structure "Class" qui contient les métadonnées
    auto* infosClasse = new Class();

    // 4. Lier le type LLVM opaque à la structure
    infosClasse->structType = typeOpaqueLLVM;

    // 5. Initialiser l'héritage
    const auto& heritage = noeudClass->getHeritage();
    if (!heritage.empty()) {
        infosClasse->parentHeritage = heritage[0];
    } else {
        infosClasse->parentHeritage = nullptr;
    }

    // 6. Préparer les registres internes (vides pour l'instant, seront remplis en Passe 2)
    infosClasse->registreVariable = new RegistreVariable();
    infosClasse->registreFonctionLocale = new RegistreFonctionLocale();

    // La VTable sera générée plus tard lors de la résolution des méthodes virtuelles
    infosClasse->vtable = nullptr;

    // 7. Enregistrer la classe dans le registre global du compilateur
    _contextGenCode->registreClass->enregistrer(nomClasse, infosClasse);

    // 8. Visiter le corps de la classe pour remplir ses registres (méthodes, etc.)
    std::string ancienneClasse = _contextGenCode->nomClasseCourante;
    _contextGenCode->nomClasseCourante = nomClasse;

    for (auto* membre : noeudClass->getListMembres()) {
        if (membre->getTypeGenere() == NoeudTypeGenere::DeclarationFonction) {
            membre->accept(this);
        }
        else if (membre->getTypeGenere() == NoeudTypeGenere::DeclarationVariable) {
            auto* declVar = static_cast<NoeudDeclarationVariable*>(membre);
            Token token;
            token.value = declVar->getNom();
            infosClasse->registreVariable->enregistrer(token, Symbole(nullptr, declVar->getType()));
            
            if (declVar->getExpression() != nullptr) {
                infosClasse->memberInitializers[declVar->getNom()] = declVar->getExpression();
            }
        }
    }

    // Aussi visiter les constructeurs si nécessaire :
    for (auto* constructeur : noeudClass->getConstructeur()) {
        constructeur->accept(this);
    }

    _contextGenCode->nomClasseCourante = ancienneClasse;
}
