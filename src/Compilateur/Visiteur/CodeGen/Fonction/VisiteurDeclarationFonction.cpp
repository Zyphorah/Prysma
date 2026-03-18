#include "Compilateur/LLVM/GestionFonction.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"

#include <memory>

void VisiteurGeneralGenCode::visiter(NoeudDeclarationFonction* noeudDeclarationFonction) 
{
    auto generateur = GenerateurDeclarationFonction::creer(_contextGenCode, noeudDeclarationFonction, this);
    generateur->declarerFonction();
}