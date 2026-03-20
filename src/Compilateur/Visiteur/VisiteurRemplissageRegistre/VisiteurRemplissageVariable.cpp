#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Lexer/Lexer.h"

// Actuellement inutilisée, c'est pour le future 

// Cette classe a pour but de remplir le registre de variable global ce trouvant à l'extérieur d'une fonction 
void VisiteurRemplissageRegistre::visiter(NoeudDeclarationVariable* noeudDeclarationVariable)
{
    Token token; 
    IType* type = noeudDeclarationVariable->getType();
    token.value = noeudDeclarationVariable->getNom();
   
    _contextGenCode->getRegistreVariable()->enregistrer(token, Symbole(nullptr, type));
}