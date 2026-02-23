#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"


void VisiteurRemplissageRegistre::visiter(NoeudDeclarationVariable* noeudDeclarationVariable)
{
    Token token; 
    IType* type = noeudDeclarationVariable->getType();
    token.value = noeudDeclarationVariable->getNom();
   
    Symbole symboleVariable(nullptr, type);
}