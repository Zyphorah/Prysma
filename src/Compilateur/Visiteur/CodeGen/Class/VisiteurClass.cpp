#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include <string>

void VisiteurGeneralGenCode::visiter(NoeudClass* noeudClass)
{
    std::string ancienneClasse = _contextGenCode->getNomClasseCourante();
    _contextGenCode->modifierNomClasseCourante( noeudClass->getNomClass().value);

    for (auto* membre : noeudClass->getListMembres()) {
        if (prysma::isa<NoeudDeclarationFonction>(membre)) {
            membre->accept(this);
        }
    }

    for (auto* constructeur : noeudClass->getConstructeur()) {
        constructeur->accept(this);
    }

    _contextGenCode->modifierNomClasseCourante( ancienneClasse);
}
