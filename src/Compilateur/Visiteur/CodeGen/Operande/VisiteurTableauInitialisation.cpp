#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"

void VisiteurGeneralGenCode::visiter(NoeudTableauInitialisation* noeudInitialisation [[maybe_unused]])
{
    // On ne peut pas vraiment initialiser un tableau en tant que "constante temporaire"
    // car les tableaux ont besoin d'être alloués. Ce visiteur sera appelé lors
    // de l'évaluation d'une expression qui contient [1, 2, 3, ...].
    // Pour l'instant, retourner nullptr - le vrai traitement doit se faire
    // dans VisiteurDeclarationVariable qui connaît le type du tableau.
    
    _contextGenCode->valeurTemporaire.adresse = nullptr;
    _contextGenCode->valeurTemporaire.type = nullptr;
}
