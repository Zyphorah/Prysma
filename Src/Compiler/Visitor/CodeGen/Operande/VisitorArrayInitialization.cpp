#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"

void GeneralVisitorGenCode::visiter(NodeArrayInitialization* nodeInitialization [[maybe_unused]])
{
    // On ne peut pas vraiment initialiser un array en tant que "constante temporaire"
    // car les arrayx ont besoin d'être alloués. Ce visitor sera callé lors
    // de l'évaluation d'une expression qui contient [1, 2, 3, ...].
    // Pour l'instant, returnner nullptr - le vrai traitement doit se faire
    // dans VisitorDeclarationVariable qui connaît le type du array.
    
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), nullptr, _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}
