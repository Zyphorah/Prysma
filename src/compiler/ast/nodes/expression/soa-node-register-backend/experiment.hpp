#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/lexer/lexer.h"
#include <iostream>
#include <llvm-18/llvm/ADT/ArrayRef.h>
#include "compiler/visitor/interfaces/i_visitor.h"
#include <string>
#include <type_traits>

/*******************************************/

// À REFACTORISER EN .h ET .cpp

/*******************************************/

#if __cplusplus >= 201703L
    #define PRYSMA_NODISCARD PRYSMA_NODISCARD
#elif defined(__GNUC__) || defined(__clang__)
    #define PRYSMA_NODISCARD __attribute__((warn_unused_result)) inline
#else
    #define PRYSMA_NODISCARD inline
#endif

/*******************************************/

using node_id_t = std::size_t;

/*******************************************/

// DU CODE DE DÉCEMBRE 2025, ASSEZ VIEUX ET TRÈS "COMPLEXE" POUR RIEN

template<typename T>
class sparse_set {
private:
    static constexpr std::size_t DEFAULT_DENSE_CAPACITY = 2048;
    static constexpr std::size_t DEFAULT_SPARSE_CAPACITY = 16384;

    using ContainedType = T;

    inline void error_not_enough_capacity(const std::string& context, size_t required, size_t actual) {
        std::cerr << "[ERROR] " << context
            << " | Required size: " << required
            << ", Actual size: " << actual << std::endl;
    }

    inline void error_entity_has_no_component(std::size_t entity_id) {
        std::cerr << "[ERROR] Entity " << entity_id
            << " does not have a component!" << std::endl;
    }

    inline void error_entity_already_has_component(std::size_t entity_id) {
        std::cerr << "[ERROR] Entity " << entity_id
            << " already has a component!" << std::endl;
    }

    inline constexpr bool is_valid_entity_id(std::size_t entity_id) const noexcept {
        return entity_id < sparse_.size();
    }

public:
    sparse_set(std::size_t init_dense_capacity = DEFAULT_DENSE_CAPACITY,
        std::size_t init_sparse_capacity = DEFAULT_SPARSE_CAPACITY) {
        dense_.reserve(init_dense_capacity);
        reverse_.reserve(init_dense_capacity);
        sparse_.reserve(init_sparse_capacity);
        sparse_.resize(init_sparse_capacity, SIZE_MAX);
    }

    sparse_set(const sparse_set&) = default;
    sparse_set& operator=(const sparse_set&) = default;
    sparse_set(sparse_set&&) noexcept = default;
    sparse_set& operator=(sparse_set&&) noexcept = default;
    ~sparse_set() = default;


public:
    template<typename U>
    void insert(std::size_t entity_id, U&& component) noexcept {
        if (!is_valid_entity_id(entity_id)) {
            error_not_enough_capacity(
                "Sparse vector too small",
                entity_id + 1,
                sparse_.size()
            ); return;
        }

        if (contains(entity_id)) {
            error_entity_already_has_component(entity_id);
            return;
        }

        dense_.push_back(std::forward<U>(component));
        std::size_t component_index = dense_.size() - 1;

        sparse_[entity_id] = component_index;
        reverse_.push_back(entity_id);
    }

    template<typename... Ts, typename U>
    auto batch_insert(Ts... entity_ids, const U& component) noexcept ->
        std::enable_if_t<(is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0),
        void> { (insert(entity_ids, component), ...); }

    auto emplace(std::size_t entity_id) noexcept ->
        std::enable_if_t<std::is_default_constructible_v<T>, void> {
        if (!is_valid_entity_id(entity_id)) {
            error_not_enough_capacity(
                "Sparse vector too small",
                entity_id + 1,
                sparse_.size()
            ); return;
        }

        std::cout << "Emplacing new component for entity " << entity_id << std::endl;

        if (contains(entity_id)) {
            error_entity_already_has_component(entity_id);
            return;
        }

        dense_.emplace_back();
        std::size_t component_index = dense_.size() - 1;

        sparse_[entity_id] = component_index;
        reverse_.push_back(entity_id);
    }

    template<typename... Ts>
    auto batch_emplace(Ts... entity_ids) noexcept ->
        std::enable_if_t<std::is_default_constructible_v<T>
        && (is_index_type_v<Ts> && ...) && (sizeof...(Ts) > 0),
        void> { (emplace(entity_ids), ...); }

    void remove_swap(std::size_t entity_id) {
        if (!is_valid_entity_id(entity_id)) {
            error_not_enough_capacity(
                "Sparse vector too small",
                entity_id + 1,
                sparse_.size()
            ); return;
        }

        if (!contains(entity_id)) {
            error_entity_has_no_component(entity_id);
            return;
        }

        std::size_t component_index = sparse_[entity_id];
        std::size_t last_component_index = dense_.size() - 1;
        std::size_t last_entity_id = reverse_[last_component_index];

        std::swap(dense_[component_index], dense_[dense_.size() - 1]);
        dense_.pop_back();

        std::swap(reverse_[component_index], reverse_[last_component_index]);
        reverse_.pop_back();

        sparse_[last_entity_id] = component_index;
        sparse_[entity_id] = SIZE_MAX;
    }

    template<typename... Ts>
    auto batch_remove_swap(Ts... entity_ids) noexcept ->
        std::enable_if_t<((is_index_type_v<Ts>) && ...)
        && (sizeof...(Ts) > 0), void> { (remove_swap(entity_ids), ...); }

public:
    bool contains(std::size_t entity_id) const {
        return is_valid_entity_id(entity_id) && sparse_[entity_id] != SIZE_MAX;
    }

    template<typename... Ts>
    auto batch_contains(Ts... entity_ids) const noexcept ->
        std::enable_if_t<(is_index_type_v<Ts> && ...)
        && (sizeof...(Ts) > 0), bool> { return (contains(entity_ids) && ...); }

    std::size_t count() const noexcept { return dense_.size(); }
    std::size_t capacity() const noexcept { return sparse_.capacity(); }
    std::size_t empty() const noexcept { return sparse_.empty(); }

    void clear_sparse() noexcept { sparse_.clear(); }
    void clear_dense() noexcept { dense_.clear(); }
    void clear_binding() noexcept { reverse_.clear(); }

    void reserve(std::size_t new_capacity) noexcept { sparse_.reserve(new_capacity); }

    void shrink_sparse_to_fit() noexcept { sparse_.shrink_to_fit(); }
    void shrink_dense_to_fit() noexcept { dense_.shrink_to_fit(); }
    void shrink_binding_to_fit() noexcept { reverse_.shrink_to_fit(); }

public:
    auto begin() noexcept { return dense_.begin(); }
    auto end() noexcept { return dense_.end(); }
    auto begin() const noexcept { return dense_.begin(); }
    auto end() const noexcept { return dense_.end(); }

public:
    T* operator[](std::size_t entity_id) noexcept {
        if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
            error_not_enough_capacity(
                "Sparse vector too small or entity has no component",
                entity_id + 1,
                sparse_.size()
            );
            return nullptr;
        }
        return &dense_[sparse_[entity_id]];
    }

    const T* operator[](std::size_t entity_id) const noexcept {
        if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
            error_not_enough_capacity(
                "Sparse vector too small or entity has no component",
                entity_id + 1,
                sparse_.size()
            );
            return nullptr;
        }
        return &dense_[sparse_[entity_id]];
    }

    T* get(std::size_t entity_id) & noexcept {
        if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
            error_not_enough_capacity(
                "Sparse vector too small or entity has no component",
                entity_id + 1,
                sparse_.size()
            );
            return nullptr;
        }
        return &dense_[sparse_[entity_id]];
    }

    const T* get(std::size_t entity_id) const& noexcept {
        if (!is_valid_entity_id(entity_id) || sparse_[entity_id] == SIZE_MAX) {
            error_not_enough_capacity(
                "Sparse vector too small or entity has no component",
                entity_id + 1,
                sparse_.size()
            );
            return nullptr;
        }
        return &dense_[sparse_[entity_id]];
    }

private:
    std::vector<std::size_t> sparse_;
    std::vector<std::size_t> reverse_;
    std::vector<T> dense_;
};


/*******************************************/


using of_names = std::string;
using of_children = std::vector<std::size_t>;
using of_nodegen_types = llvm::ArrayRef<INode*>;

/*******************************************/

enum class NodeTypeGenerated {
    Instruction, 
    CallFunction,
    ArgFunction,
    DeclarationFunction,
    Return,
    AssignmentVariable,
    DeclarationVariable,
    RefVariable,
    UnRefVariable,
    Identifiant,
    AssignmentArray,
    ArrayInitialization,
    ReadingArray,
    Class,
    CallObject,
    AccesAttribute,
    DeclarationObject,
    If,
    New,
    Delete,
    Include,
    While,
    Operation,
    Literal,
    Negation,
    String,
};

/*******************************************/

struct NODEGEN_TYPE_COMPONENT_TAG {};

struct NAME_COMPONENT_TAG {};
struct CHILD_COMPONENT_TAG {};

/*******************************************/

// template<typename T = void*>
// struct sparse_set {
//     auto dummy_get(node_id_t id) { return Token{}; } 
// };

/*******************************************/

struct node_components_registry {

};

struct NodeComponentRegistry {

};

struct NodeRegistry {
   // il faut empecher les operateurs de création pour sécuriser le singleton ;)

   NodeRegistry() = default;

public:
    static NodeRegistry& instance() { // peut etre repenser l'approche singleton..?
        static NodeRegistry inst;
        return inst;
    }


public:
    template<typename COMPONENT_TAG> struct mapper;


public:
    template<typename T, typename COMPONENT_TAG>
    auto insert(node_id_t id, T&& component) noexcept -> void {
        mapper<COMPONENT_TAG>::get(*this).insert(id, std::forward<T>(component));
    }

    template<typename COMPONENT_TAG>
    auto emplace(node_id_t id) noexcept -> void {
        mapper<COMPONENT_TAG>::get(*this).emplace(id);
    }

    template<typename COMPONENT_TAG>
    auto get(node_id_t id) noexcept -> decltype(auto) {
        return mapper<COMPONENT_TAG>::get(*this).get(id);
    }

    template<typename COMPONENT_TAG>
    auto remove(node_id_t id) noexcept -> void {
        mapper<COMPONENT_TAG>::get(*this).remove(id);
    }


private:
    sparse_set<of_nodegen_types> nodegen_types;

    sparse_set<of_names> names;
    sparse_set<of_children> children;

};

/*******************************************/

template<> struct NodeRegistry::mapper<NODEGEN_TYPE_COMPONENT_TAG> {
    static decltype(auto) get(NodeRegistry& reg) { return reg.nodegen_types; }
};

template<> struct NodeRegistry::mapper<NAME_COMPONENT_TAG> {
    static decltype(auto) get(NodeRegistry& reg) { return reg.names; }
};

template<> struct NodeRegistry::mapper<CHILD_COMPONENT_TAG> {
    static decltype(auto) get(NodeRegistry& reg) { return reg.children; }
};

/*******************************************/

// C'EST CELLE-CI QUE NOUS ALLONS UTILISER, PAS CELLE D'EN BAS
// on garde la logique de création et d'otention à l'extérieur du Node
// c'est beaucoup plus propre, extensible et ça simplifie énormément 
// le travail par la suite. 

struct NodeCallFonction2 : public INode {
public:
    PRYSMA_NODISCARD constexpr auto get() const noexcept -> node_id_t {
         return this->nodeId_;
    }

private:
    node_id_t nodeId_;
};


/*******************************************/




// METTRE DEHORS DE LA CLASSE, LE NodeCallFonction ne devrait pas s'occuper de sa construction

// registry_.insert<NAME_COMPONENT_TAG>(nodeId_, std::move(p_nomFunction));
// registry_.insert<CHILD_COMPONENT_TAG>(nodeId_, p_children);

struct NodeCallFonction : public INode {
public:
    NodeCallFonction() = default;

    NodeCallFonction(NodeRegistry& p_registry, Token p_nomFunction, llvm::ArrayRef<INode*> p_children = {}) : registry_(p_registry) {
        registry_.insert<NAME_COMPONENT_TAG>(nodeId_, std::move(p_nomFunction));
        registry_.insert<CHILD_COMPONENT_TAG>(nodeId_, p_children);
   }
        
    ~NodeCallFonction() override = default;

public:
    void accept(IVisitor* visitor) override {}

    PRYSMA_NODISCARD static bool classof(const INode* node) {
        return node->getGeneratedType() == NodeTypeGenerated::CallFunction;
    }

    PRYSMA_NODISCARD auto getGeneratedType() -> NodeTypeGenerated const override {
        return registry_.get<NODEGEN_TYPE_COMPONENT_TAG>(nodeId_);
    }

    PRYSMA_NODISCARD auto getNomFunction() -> const Token& const {
        return registry_.get<NAME_COMPONENT_TAG>(nodeId_);
    }

    PRYSMA_NODISCARD auto getChildren() -> llvm::ArrayRef<INode*> const {
        return registry_.get<CHILD_COMPONENT_TAG>(nodeId_);
    }

private:
    node_id_t nodeId_;

};


/*******************************************/

// ORIGINAL IMPLEMENTATION (GENERATED VIA NINJA2)

// class NodeCallFunction_ : public INode {
// private:
//     NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::CallFunction;
//     Token _nomFunction;
//     llvm::ArrayRef<INode*> _children;

// public:
//     NodeCallFunction_() = default;    NodeCallFunction_(Token nomFunction, llvm::ArrayRef<INode*> children = nullptr) : _nomFunction(std::move(nomFunction)), _children(children) {}    ~NodeCallFunction() override = default;

//     void accept(IVisitor* visitor) override {
//         //visitor->visiter(this);
//     }

//     PRYSMA_NODISCARD NodeTypeGenerated getGeneratedType() const override {
//         return _nodeTypeGenerated;
//     }

//     PRYSMA_NODISCARD static bool classof(const INode* node) {
//         return node->getGeneratedType() == NodeTypeGenerated::CallFunction;
//     }
//     PRYSMA_NODISCARD const Token& getNomFunction() const { return _nomFunction; }
//     PRYSMA_NODISCARD llvm::ArrayRef<INode*> getChildren() const { return _children; }
// };

/*******************************************/