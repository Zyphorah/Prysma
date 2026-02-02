#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"


NoeudDeclarationFonction::NoeudDeclarationFonction()
{}

NoeudDeclarationFonction::~NoeudDeclarationFonction()
{}



llvm::Value* NoeudDeclarationFonction :: genCode()
{
    return executerEnfants();
}