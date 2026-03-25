#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"

// Actuellement inutilisée, c'est pour le future 

// Cette classe a pour but de remplir le registry de variable global ce trouvant à l'extérieur d'une function 
void FillingVisitorRegistry::visiter(NodeDeclarationVariable* nodeDeclarationVariable)
{
    Token token; 
    IType* type = nodeDeclarationVariable->getType();
    token.value = nodeDeclarationVariable->getNom();
   
    _contextGenCode->getRegistryVariable()->enregistryr(token, Symbole(nullptr, type));
}