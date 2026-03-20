#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h" 
#include "Compilateur/AST/AST_Genere.h" 
#include <filesystem>

void VisiteurRemplissageRegistre::visiter(NoeudInclude* noeudInclude)
{
    std::filesystem::path cheminFichierParent(_contextGenCode->getCheminFichierCourant());
    std::filesystem::path repertoireParent = cheminFichierParent.parent_path();
    std::filesystem::path cheminAbsolu = repertoireParent / noeudInclude->getPath();
    _orchestrateur->inclureFichier(cheminAbsolu.string());
}

