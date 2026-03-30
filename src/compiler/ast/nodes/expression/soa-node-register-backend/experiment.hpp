#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/lexer/lexer.h"
#include <iostream>
#include <llvm-18/llvm/ADT/ArrayRef.h>
#include "compiler/visitor/interfaces/i_visitor.h"
#include <string>

/*******************************************/

// À REFACTORISER EN .h ET .cpp

/*******************************************/

#if __cplusplus >= 201703L
    #define PRYSMA_NODISCARD [[nodiscard]]
#elif defined(__GNUC__) || defined(__clang__)
    #define PRYSMA_NODISCARD __attribute__((warn_unused_result)) inline
#else
    #define PRYSMA_NODISCARD inline
#endif

/*******************************************/

using node_id_t = std::size_t;

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

template<typename T = void*>
struct sparse_set {
    auto dummy_get(node_id_t id) { return Token{}; } 
};

/*******************************************/

struct node_registry {
   // il faut empecher les operateurs de création pour sécuriser le singleton ;)

public:
    static node_registry& instance() { // peut etre repenser l'approche singleton..?
        static node_registry inst;
        return inst;
    }

public:
    template<typename COMPONENT_TAG> struct mapper;

public:
    template<typename COMPONENT_TAG>
    decltype(auto) get(node_id_t id) noexcept {
        return mapper<COMPONENT_TAG>::get(*this).dummy_get(id);
    }


private:
    sparse_set<of_nodegen_types> nodegen_types;

    sparse_set<of_names> names;
    sparse_set<of_children> children;

};

/*******************************************/

template<> struct node_registry::mapper<NODEGEN_TYPE_COMPONENT_TAG> {
    static decltype(auto) get(node_registry& reg) { return reg.nodegen_types; }
};

template<> struct node_registry::mapper<NAME_COMPONENT_TAG> {
    static decltype(auto) get(node_registry& reg) { return reg.names; }
};

template<> struct node_registry::mapper<CHILD_COMPONENT_TAG> {
    static decltype(auto) get(node_registry& reg) { return reg.children; }
};

/*******************************************/

struct NodeCallFonction : public INode {
public:
    void accept(IVisitor* visitor) override {}

    PRYSMA_NODISCARD static bool classof(const INode* node) {
        return node->getGeneratedType() == NodeTypeGenerated::CallFunction;
    }

    PRYSMA_NODISCARD NodeTypeGenerated getGeneratedType() const override {
        return node_registry::instance().get<NODEGEN_TYPE_COMPONENT_TAG>(NodeId);
    }

    PRYSMA_NODISCARD const Token& getNomFunction() const {
        return node_registry::instance().get<NAME_COMPONENT_TAG>(NodeId);
    }

    PRYSMA_NODISCARD llvm::ArrayRef<INode*> getChildren() const {
        return node_registry::instance().get<CHILD_COMPONENT_TAG>(NodeId);
    }

private:
    node_id_t NodeId;

};


/*******************************************/

// ORIGINAL IMPLEMENTATION (GENERATED VIA NINJA2)

class NodeCallFunction_ : public INode {
private:
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::CallFunction;
    Token _nomFunction;
    llvm::ArrayRef<INode*> _children;

public:
    NodeCallFunction_() = default;    NodeCallFunction_(Token nomFunction, llvm::ArrayRef<INode*> children = nullptr) : _nomFunction(std::move(nomFunction)), _children(children) {}    ~NodeCallFunction() override = default;

    void accept(IVisitor* visitor) override {
        //visitor->visiter(this);
    }

    [[nodiscard]] NodeTypeGenerated getGeneratedType() const override {
        return _nodeTypeGenerated;
    }

    [[nodiscard]] static bool classof(const INode* node) {
        return node->getGeneratedType() == NodeTypeGenerated::CallFunction;
    }
    [[nodiscard]] const Token& getNomFunction() const { return _nomFunction; }
    [[nodiscard]] llvm::ArrayRef<INode*> getChildren() const { return _children; }
};

/*******************************************/