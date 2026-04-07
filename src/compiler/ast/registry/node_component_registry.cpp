#include "compiler/ast/registry/node_component_registry.h"

auto NodeComponentRegistry::getNextId() noexcept -> node_id_t {
    return ++currentId;
}

sparse_set<of_nodegen_types>&
NodeComponentRegistry::mapper<AST_NODEGEN_TYPE_COMPONENT>::get(NodeComponentRegistry& reg) {
    return reg.nodegen_types;
}

sparse_set<of_itypes>&
NodeComponentRegistry::mapper<AST_ITYPE_COMPONENT>::get(NodeComponentRegistry& reg) {
    return reg.itypes;
}

sparse_set<of_names>&
NodeComponentRegistry::mapper<AST_NAME_COMPONENT>::get(NodeComponentRegistry& reg) {
    return reg.names;
}

sparse_set<of_children>&
NodeComponentRegistry::mapper<AST_CHILD_COMPONENT>::get(NodeComponentRegistry& reg) {
    return reg.children;
}