#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include <memory>
#include <string>
#include <utility>

void VisiteurRemplissageRegistre::visiter(NoeudDeclarationFonction* noeudDeclarationFonction)
{
    IType* typeRetour = noeudDeclarationFonction->getTypeRetour();
    std::string nomFonction = noeudDeclarationFonction->getNom();
    
    if (_contextGenCode->getNomClasseCourante() != "") {
        // contexte de la class (méthode)
        auto symboleFonction = std::make_unique<SymboleFonctionLocale>();
        symboleFonction->typeRetour = typeRetour;
        symboleFonction->noeud = noeudDeclarationFonction;

        std::string nomClasse = _contextGenCode->getNomClasseCourante();
        auto* classInfo = _contextGenCode->getRegistreClass()->recuperer(nomClasse).get();
        classInfo->getRegistreFonctionLocale()->enregistrer(nomFonction, std::move(symboleFonction));
    } else {
        // contexte global (fonction globale)
        auto symboleFonction = std::make_unique<SymboleFonctionGlobale>();
        symboleFonction->typeRetour = typeRetour;
        symboleFonction->noeud = noeudDeclarationFonction;

        _contextGenCode->getRegistreFonctionGlobale()->enregistrer(nomFonction, std::move(symboleFonction));
    }
}