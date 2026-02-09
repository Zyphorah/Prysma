#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudRetour.h"

void VisiteurGeneralGenCode::visiter(NoeudRetour* noeudReturn)
{
    noeudReturn->_valeurRetour->accept(this);

    TokenType typeRetourToken = _contextGenCode->returnContextCompilation->recupererContext();
    llvm::Type* typeRetour = _contextGenCode->registreType->recuperer(typeRetourToken);
    
    llvm::Value* valeurRetour = _contextGenCode->backend->getBuilder().CreateFPToSI(_contextGenCode->valeurTemporaire, typeRetour);
    
    _contextGenCode->backend->getBuilder().CreateRet(valeurRetour);
    _contextGenCode->valeurTemporaire = valeurRetour;
} 