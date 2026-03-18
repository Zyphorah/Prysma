#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionFonction.h"

void VisiteurGeneralGenCode::visiter(NoeudAppelFonction* noeudAppelFonction)
{
    auto generateur = GenerateurAppelFonction::creer(_contextGenCode, this);
    generateur->genererAppelFonction(noeudAppelFonction);
}