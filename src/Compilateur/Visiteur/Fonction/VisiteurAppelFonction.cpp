#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"




void VisiteurGeneralGenCode::visiter(NoeudAppelFonction* noeudAppelFonction)
{
    GestionFonction gestionnaireFonction(_contextGenCode, this);
    gestionnaireFonction.genererAppelFonction(noeudAppelFonction);
}