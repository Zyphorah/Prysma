#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"

#include <memory>

void VisiteurGeneralGenCode::visiter(NoeudDeclarationFonction* noeudDeclarationFonction) 
{
    GestionFonction generateurFonction(_contextGenCode,noeudDeclarationFonction,this);
    generateurFonction.declarerFonction();
}