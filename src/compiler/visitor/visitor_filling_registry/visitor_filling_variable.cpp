#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"

// Currently unused, for the future

// This class is intended to fill the global variable registry located outside a function
void FillingVisitorRegistry::visiter(NodeDeclarationVariable* nodeDeclarationVariable)
{
    Token token; 
    IType* type = nodeDeclarationVariable->getType();
    token.value = nodeDeclarationVariable->getNom().value;
   
    _contextGenCode->getRegistryVariable()->registerVariable(token, Symbol(nullptr, type));
}