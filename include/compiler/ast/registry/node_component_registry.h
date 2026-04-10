#pragma once

#include <llvm-18/llvm/ADT/ArrayRef.h>
#include <stdexcept>
#include <utility>
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/macros/prysma_nodiscard.h"
#include "compiler/macros/prysma_likely.h"
#include "compiler/lexer/lexer.h"

#include "compiler/data_structures/sparse_set.h"

using node_id_t = std::size_t;

struct NODEGEN_TYPE_COMPONENT_TAG;
struct NAME_COMPONENT_TAG;
struct CHILD_COMPONENT_TAG;

struct AST_NODEGEN_TYPE_COMPONENT;
struct AST_ITYPE_COMPONENT;
struct AST_NAME_COMPONENT;

struct AST_CHILD_COMPONENT;         // POUR SÉPARER LES ELEMENTS DES ENFANTS DU SYSTÈME
struct AST_ARRAY_ELEMENT_COMPONENT; // AFIN DE PERMETTRE ITÉRER SUR L'ARRAY DES CHILD DU SYSTÈME PAR EX

struct AST_NODE_RETURN_COMPONENT;


using of_nodegen_types = NodeTypeGenerated;
using of_itypes = IType*; // sont alloc dans le pool de bump alloc donc c'est good

using of_names = Token;
using of_children = llvm::ArrayRef<INode*>; // les enfants sont alloc dans pool de bump alloc
using of_arr_elements = llvm::ArrayRef<INode*>; 

using of_return_nodes = INode*;


#if 0
using of_NodeTypeGenerated = NodeTypeGenerated;
using of_IType = IType*;

using of_Token = Token;
#endif


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
    auto get(node_id_t id) noexcept -> auto&;

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
    sparse_set<of_arr_elements> arr_elements;

    sparse_set<of_return_nodes> return_nodes;
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

template<>
struct NodeComponentRegistry::mapper<AST_ARRAY_ELEMENT_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_arr_elements>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::mapper<AST_NODE_RETURN_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_return_nodes>& get(NodeComponentRegistry& reg);
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
auto NodeComponentRegistry::get(node_id_t id) noexcept -> auto& {
    auto* ptr = mapper<COMPONENT_TAG>::get(*this).get(id);

    if (ptr) PRYSMA_LIKELY_BRANCH 
        return *ptr; // ou std::optional tout dépendamment ce que j'opterai
    throw std::out_of_range("AST component not found"); // crash manuel

    // la logique est parfait mais c'est surtout de savoir si le sparse set
    // return un ptr ou une ref.
    
    // C'EST UN PATCH TEMPORAIRE EN ATTENDANT QUE LE SPARSE SET SOIT STABLE
    // ON RETOURNE UNE RÉFÉRENCE VERS LA RESSOURCE POINTÉ. DANS L'IDÉAL, IL 
    // FAUDRAIT PROBABLEMENT QUE LE SPARSE SET RETOURNE UNE RÉFÉRENCE AU LIEU
    // D'UN POINTEUR. CE N'EST PAS CLAIR, IL FAUDRAIT EN DISCUTER. 
    //return mapper<COMPONENT_TAG>::get(*this).get(id);
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::remove(node_id_t id) noexcept -> void {
    mapper<COMPONENT_TAG>::get(*this).remove(id);
}