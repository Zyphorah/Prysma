#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include <string>

void VisiteurGeneralGenCode::visiter(NoeudClass* noeudClass)
{
    std::string ancienneClasse = _contextGenCode->getNomClasseCourante();
    _contextGenCode->modifierNomClasseCourante( noeudClass->getNomClass().value);

    for (auto* membre : noeudClass->getListMembres()) {
        if (membre->getTypeGenere() == NoeudTypeGenere::DeclarationFonction) {
            membre->accept(this);
        }
    }

    for (auto* constructeur : noeudClass->getConstructeur()) {
        constructeur->accept(this);
    }

    _contextGenCode->modifierNomClasseCourante( ancienneClasse);
}
