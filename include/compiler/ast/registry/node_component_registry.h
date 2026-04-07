#pragma once

#include <llvm-18/llvm/ADT/ArrayRef.h>
#include <utility>
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/macros/prysma_nodiscard.h"
#include "compiler/lexer/lexer.h"

#include "compiler/data_structures/sparse_set.h"

using node_id_t = std::size_t;

struct NODEGEN_TYPE_COMPONENT_TAG;
struct NAME_COMPONENT_TAG;
struct CHILD_COMPONENT_TAG;

struct AST_NODEGEN_TYPE_COMPONENT;
struct AST_ITYPE_COMPONENT;
struct AST_NAME_COMPONENT;
struct AST_CHILD_COMPONENT;

using of_nodegen_types = NodeTypeGenerated;
using of_itypes = IType*; // sont alloc dans le pool de bump alloc donc c'est good

using of_names = Token;
using of_children = llvm::ArrayRef<INode*>; // les enfants sont alloc dans pool de bump alloc


struct NodeComponentRegistry {
    NodeComponentRegistry() = default;

public:
    template<typename COMPONENT_TAG> struct mapper;

public:
    template<typename COMPONENT_TAG, typename T>
    auto insert(node_id_t id, T&& component) noexcept -> void;

    template<typename COMPONENT_TAG>
    auto emplace(node_id_t id) noexcept -> void;

    template<typename COMPONENT_TAG>
    auto get(node_id_t id) noexcept -> decltype(auto);

    template<typename COMPONENT_TAG>
    auto remove(node_id_t id) noexcept -> void;

public:
    auto getNextId() noexcept -> node_id_t;

private:
    node_id_t currentId = 0;

private:
    sparse_set<of_nodegen_types> nodegen_types;
    sparse_set<of_itypes> itypes;

    sparse_set<of_names> names;
    sparse_set<of_children> children;
};



template<>
struct NodeComponentRegistry::mapper<AST_NODEGEN_TYPE_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_nodegen_types>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::mapper<AST_ITYPE_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_itypes>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::mapper<AST_NAME_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_names>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::mapper<AST_CHILD_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_children>& get(NodeComponentRegistry& reg);
};



template<typename COMPONENT_TAG, typename T>
auto NodeComponentRegistry::insert(node_id_t id, T&& component) noexcept -> void {
    mapper<COMPONENT_TAG>::get(*this).insert(id, std::forward<T>(component));
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::emplace(node_id_t id) noexcept -> void {
    mapper<COMPONENT_TAG>::get(*this).emplace(id);
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::get(node_id_t id) noexcept -> decltype(auto) {
    return mapper<COMPONENT_TAG>::get(*this).get(id);
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::remove(node_id_t id) noexcept -> void {
    mapper<COMPONENT_TAG>::get(*this).remove(id);
}