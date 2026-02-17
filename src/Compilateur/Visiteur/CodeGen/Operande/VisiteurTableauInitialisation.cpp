#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include <llvm/IR/Constants.h>

void VisiteurGeneralGenCode::visiter(NoeudTableauInitialisation* noeudTableauInit)
{
    if (noeudTableauInit == nullptr) {
        _contextGenCode->valeurTemporaire = nullptr;
        return;
    }

    // On ne peut pas vraiment initialiser un tableau en tant que "constante temporaire"
    // car les tableaux ont besoin d'être alloués. Ce visiteur sera appelé lors
    // de l'évaluation d'une expression qui contient [1, 2, 3, ...].
    // Pour l'instant, retourner nullptr - le vrai traitement doit se faire
    // dans VisiteurDeclarationVariable qui connaît le type du tableau.
    
    _contextGenCode->valeurTemporaire = nullptr;
}
