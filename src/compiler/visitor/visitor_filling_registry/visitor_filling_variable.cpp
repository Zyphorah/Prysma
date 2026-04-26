#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"

// Currently unused, for the future

// This class is intended to fill the global variable registry located outside a function
void FillingVisitorRegistry::visiter(NodeDeclarationVariable* nodeDeclarationVariable)
{
    auto& nodeDeclVarData = _contextGenCode->getNodeComponentRegistry()
        ->get<NodeDeclarationVariableComponents>(nodeDeclarationVariable->getNodeId());

    Token token; 
    IType* type = nodeDeclVarData.getType();
    token.value = nodeDeclVarData.getName().value;
   
    _contextGenCode->getRegistryVariable()->registerVariable(token, Symbol(nullptr, type));
}