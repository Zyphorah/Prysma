#include "compiler/ast/registry/node_component_registry.h"

auto NodeComponentRegistry::getNextId() noexcept -> node_id_t {
    return ++globalId;
}

template<typename StoredType> sparse_set<StoredType>&
NodeComponentRegistry::resolve<StoredType>::get(NodeComponentRegistry& reg) {
    return std::get<sparse_set<StoredType>>(reg.storage_);
}


// sparse_set<of_nodegen_types>&
// NodeComponentRegistry::resolve<AST_NODEGEN_TYPE_COMPONENT>::get(NodeComponentRegistry& reg) {
//     return reg.nodegen_types;
// }

// sparse_set<of_itypes>&
// NodeComponentRegistry::resolve<AST_ITYPE_COMPONENT>::get(NodeComponentRegistry& reg) {
//     return reg.itypes;
// }

// sparse_set<of_names>&
// NodeComponentRegistry::resolve<AST_NAME_COMPONENT>::get(NodeComponentRegistry& reg) {
//     return reg.names;
// }

// sparse_set<of_children>&
// NodeComponentRegistry::resolve<AST_CHILD_COMPONENT>::get(NodeComponentRegistry& reg) {
//     return reg.children;
// }

// sparse_set<of_arr_elements>&
// NodeComponentRegistry::resolve<AST_ARRAY_ELEMENT_COMPONENT>::get(NodeComponentRegistry& reg) {
//     return reg.arr_elements;
// }

// sparse_set<of_return_nodes>&
// NodeComponentRegistry::resolve<AST_NODE_RETURN_COMPONENT>::get(NodeComponentRegistry& reg) {
//     return reg.return_nodes;
}