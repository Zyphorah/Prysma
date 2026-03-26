#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"

// Currently unused, for the future

// This class is intended to fill the global variable registry located outside a function
void FillingVisitorRegistry::visiter(NodeDeclarationVariable* nodeDeclarationVariable)
{
    Token token; 
    IType* type = nodeDeclarationVariable->getType();
    token.value = nodeDeclarationVariable->getNom();
   
    _contextGenCode->getRegistryVariable()->registerVariable(token, Symbol(nullptr, type));
}