#include "compiler/ast/registry/node_component_registry.h"

auto NodeComponentRegistry::getNextId() noexcept -> node_id_t {
    return ++currentId;
}

sparse_set<of_nodegen_types>&
NodeComponentRegistry::mapper<NODEGEN_TYPE_COMPONENT_TAG>::get(NodeComponentRegistry& reg) {
    return reg.nodegen_types;
}

sparse_set<of_names>&
NodeComponentRegistry::mapper<NAME_COMPONENT_TAG>::get(NodeComponentRegistry& reg) {
    return reg.names;
}

sparse_set<of_children>&
NodeComponentRegistry::mapper<CHILD_COMPONENT_TAG>::get(NodeComponentRegistry& reg) {
    return reg.children;
}