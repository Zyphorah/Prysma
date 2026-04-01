#pragma once

#include <utility>
#include "/compiler/macros/prysma_nodiscard.h"

using node_id_t = std::size_t;

template<typename T>
class sparse_set {}; // TEMPORAIRE EN ATTENDANT D'AJOUTER LE SPARSE SET

struct NODEGEN_TYPE_COMPONENT_TAG;
struct NAME_COMPONENT_TAG;
struct CHILD_COMPONENT_TAG;

struct of_nodegen_types;
struct of_names;
struct of_children;

struct NodeComponentRegistry {
    NodeComponentRegistry() = default;

public:
    template<typename COMPONENT_TAG> struct mapper;

public:
    template<typename T, typename COMPONENT_TAG>
    auto insert(node_id_t id, T&& component) noexcept -> void;

    template<typename COMPONENT_TAG>
    auto emplace(node_id_t id) noexcept -> void;

    template<typename COMPONENT_TAG>
    auto get(node_id_t id) noexcept -> decltype(auto);

    template<typename COMPONENT_TAG>
    auto remove(node_id_t id) noexcept -> void;

private:
    sparse_set<of_nodegen_types> nodegen_types;
    sparse_set<of_names> names;
    sparse_set<of_children> children;
};


template<>
struct NodeComponentRegistry::mapper<NODEGEN_TYPE_COMPONENT_TAG> {
    PRYSMA_NODISCARD static decltype(auto) get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::mapper<NAME_COMPONENT_TAG> {
    PRYSMA_NODISCARD static decltype(auto) get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::mapper<CHILD_COMPONENT_TAG> {
    PRYSMA_NODISCARD static decltype(auto) get(NodeComponentRegistry& reg);
};


template<typename T, typename COMPONENT_TAG>
auto NodeComponentRegistry::insert(node_id_t id, T&& component) noexcept -> void {
    //mapper<COMPONENT_TAG>::get(*this).insert(id, std::forward<T>(component));
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::emplace(node_id_t id) noexcept -> void {
    //mapper<COMPONENT_TAG>::get(*this).emplace(id);
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::get(node_id_t id) noexcept -> decltype(auto) {
    //return mapper<COMPONENT_TAG>::get(*this).get(id);
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::remove(node_id_t id) noexcept -> void {
    //mapper<COMPONENT_TAG>::get(*this).remove(id);
}