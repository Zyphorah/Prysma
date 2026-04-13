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

// BON, C'EST JUSTE UN COMMIT TEMPORAIRE, JE FAIS LA STRUCTURE DEMAIN OU APRÈS DEMAIN


/************************************************************************/

enum class NodeTypeGenerated {    Instruction,    CallFunction,    ArgFunction,    DeclarationFunction,    Return,    AssignmentVariable,    DeclarationVariable,    RefVariable,    UnRefVariable,    Identifiant,    AssignmentArray,    ArrayInitialization,    ReadingArray,    Class,    CallObject,    AccesAttribute,    DeclarationObject,    If,    New,    Delete,    Include,    While,    Operation,    Literal,    Negation,    String,};


using node_id_t = std::size_t;

// FAIRE NAMESPACE OU SOME SHIT POUR ENPASULER LES TAGS


/************************************************************************/


struct NODEGEN_TYPE_COMPONENT_TAG;
struct NAME_COMPONENT_TAG;
struct CHILD_COMPONENT_TAG;


/************************************************************************/

// un sparse set par type et ensuite, je fais des tableaux de ptrs pour chacunes
// des catégories, genre Token*[] names qui stocke des ptrs vers les cases du sparse
// set ou les names sont. a chaque ajout avec le tag name, on ajoute le ptr dans l'array
// et ensuite, on ajoute l'élément dans le sparse set en question.


/*
namespace prysma::rtag {

struct node_id {};
struct token {};

struct name {};
struct type {};

struct visibility {};

struct expression {};
struct condition {};

struct index {};
struct index_expression {};

struct arguments {};

struct left {};
struct right {};
struct operand {};

struct body {};

struct block_if {};
struct block_else {};
struct block_end {};

struct heritage {};
struct members {};
struct constructors {};

struct object_name {};
struct method_name {};
struct attribute_name {};

struct path {};

struct return_type {};

}
*/

namespace prysma::rtag {
    struct node_id {};
    struct token {};

    struct name {};
    struct type {};

    struct visibility {};

    struct expression {};
    struct condition {};

    struct index {};
    struct index_expression {};

    struct arguments {};

    struct left {};
    struct right {};
    struct operand {};

    struct body {};

    struct block_if {};
    struct block_else {};
    struct block_end {};

    struct heritage {};
    struct members {};
    struct constructors {};

    struct object_name {};
    struct method_name {};
    struct attribute_name {};

    struct path {};

    struct return_type {};
};

// ou littéralement faire un mapper en condition du type de noeud
// MAIS UNIQUEMENT SI ON FAIT PLUSIEURS NOEUDS, objectivement, nous
// n'aurions besoin que d'un seul noeud avec un id, pas plusieurs types.


/************************************************************************/

// stocker dans des sparse set différents
// maintenant, il s'agirait peut-être de faire un pool ou quelque
// chose a coté et pointer avec les sparse set, je ne sais pas.

// LISTE DES COMPONSANTES DU SYSTÈME

struct NodeInstructionComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Instruction;

    llvm::ArrayRef<INode*> _children;
};

struct NodeCallFunctionComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::CallFunction;

    Token _nomFunction;
    llvm::ArrayRef<INode*> _children;
};

struct NodeArgFunctionComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::ArgFunction;

    IType* _type;
    Token _nom;
};

struct NodeDeclarationFunctionComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::DeclarationFunction;

    Token _visibilite;
    IType* _typeReturn;
    Token _nom;
    llvm::ArrayRef<INode*> _arguments;
    INode* _body;
};

struct NodeReturnComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Return;

    INode* _valeurReturn;
};

struct NodeAssignmentVariableComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::AssignmentVariable;

    Token _nom;
    INode* _expression;
    Token _token;
};

struct NodeDeclarationVariableComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::DeclarationVariable;

    Token _visibilite; 
    Token _nom;
    IType* _type;
    INode* _expression; 
};

struct NodeRefVariableComponents {
     NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::RefVariable;
     Token _nomVariable;
};

struct NodeUnRefVariableComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::UnRefVariable;

    Token _nomVariable;
};

struct NodeIdentifiantComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Identifiant;
};

struct NodeAssignmentArrayComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::AssignmentArray;

    Token _nom; 
    INode* _expressionIndex;
    INode* _expression;
    Token _token;
};

struct NodeArrayInitializationComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::ArrayInitialization;

    llvm::ArrayRef<INode*> _elements;
};

struct NodeReadingArrayComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::ReadingArray;

    INode* _indexEquation;
    Token _nomArray;
};

struct NodeClassComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Class;

    llvm::ArrayRef<INode*> _Heritage;
    llvm::ArrayRef<INode*> _listMembers;
    llvm::ArrayRef<INode*> _builder;
    Token _nomClass;
};

struct NodeCallObjectComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::CallObject;

    Token _nomObject;
    Token _nomMethode;
    llvm::ArrayRef<INode*> _children;
};

struct NodeAccesAttributeComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::AccesAttribute;

    Token _nomObject;
    Token _nomAttribute;
};

struct NodeDeclarationObjectComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::DeclarationObject;

    Token _nomObject;
    IType* _typeObject;
};

struct NodeIfComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::If;

    INode* _nodeCondition;
    INode* _nodeBlocIf;
    INode* _nodeBlocElse;
    INode* _nodeBlocEndif;
};

struct NodeNewComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::New;

    llvm::ArrayRef<INode*> _arguments;
    Token _nomType;
};

struct NodeDeleteComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Delete;

    Token _nomType;
};

struct NodeIncludeComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Include;

    Token _path;
};

struct NodeWhileComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::While;

    INode* _nodeCondition;
    INode* _nodeBlocWhile;
    INode* _nodeBlocFinWhile;
};

struct NodeOperationComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Operation;

    Token _token;
    INode* _gauche;
    INode* _droite;
};

struct NodeLiteralComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Literal;

    Token _token;
};

struct NodeNegationComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::Negation;

    Token _operateur;
    INode* _operande;
};

struct NodeStringComponents {
    NodeTypeGenerated _nodeTypeGenerated = NodeTypeGenerated::String;
};

// FIN DE LA SECTION

/************************************************************************/



// namespace prysma::rtag {
//     struct source {};

//     struct child {};
//     struct name {};

//     struct condition_node {};

//     struct return_node {};

//     //struct node_return {};
//     struct node_gen_type {};
//     struct itype {};
//     struct array_element {};
// }


struct AST_NODEGEN_TYPE_COMPONENT;
struct AST_ITYPE_COMPONENT;
struct AST_NAME_COMPONENT;

struct AST_CHILD_COMPONENT;         // POUR SÉPARER LES ELEMENTS DES ENFANTS DU SYSTÈME
struct AST_ARRAY_ELEMENT_COMPONENT; // AFIN DE PERMETTRE ITÉRER SUR L'ARRAY DES CHILD DU SYSTÈME PAR EX

struct AST_NODE_RETURN_COMPONENT;


/************************************************************************/


using of_nodegen_types = NodeTypeGenerated;
using of_itypes = IType*; // sont alloc dans le pool de bump alloc donc c'est good

using of_names = Token;
using of_children = llvm::ArrayRef<INode*>; // les enfants sont alloc dans pool de bump alloc
using of_arr_elements = llvm::ArrayRef<INode*>; 

using of_return_nodes = INode*;


/************************************************************************/


#if 0
using of_NodeTypeGenerated = NodeTypeGenerated;
using of_IType = IType*;

using of_Token = Token;
#endif


/************************************************************************/


struct NodeComponentRegistry {
    NodeComponentRegistry() = default;

public:
    template<typename COMPONENT_TAG> struct resolve;

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
    // chaques threads ont leur compteur, deux registres de deux
    // threads peuvent avoir les mêmes ids mais qui sont destinés 
    // à des éléments différents. dans le cas ou il faudrait que les
    // id soient compatibles entre les différents threads, il faudrait
    // transformer le currentId en membre statique et s'assurer qu'il
    // est thread safe afin d'éviter les race ou les incrémentations invalides

private:
    sparse_set<of_nodegen_types> nodegen_types;
    sparse_set<of_itypes> itypes;

    sparse_set<of_names> names;

    sparse_set<of_children> children;
    sparse_set<of_arr_elements> arr_elements;

    sparse_set<of_return_nodes> return_nodes;

private:
    // STOCKER LES SPARSE SET PAR TYPES

    // ENSUITE, FAIRE UN VECTOR<ID> POUR CHACUNS DES CATÉGORIES
    // PAR EXEMPLE, VECTOR<ID> NAME, QUI STOCKE TOUS LES ID DE NOMS

    // C'EST TOUT SIMPLEMENT IMPOSSIBLE CAR UN NOEUD DOIT POUVOIR AVOIR
    // PLUS QUE UN ÉLÉMENT DE CHAQUE TYPES, CE QUI OBLIGE D'UTILISER 
    // DES SPARSE SETS
};


// faire une genre de table compile time ou MACRO pour éviter le boilerplate
// faut aussi garder en tête qu'on utilise Jinja2 pour la génération, donc pas fin du monde

template<>
struct NodeComponentRegistry::resolve<AST_NODEGEN_TYPE_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_nodegen_types>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::resolve<prysma::rtag::array_element> {
    PRYSMA_NODISCARD static sparse_set<of_nodegen_types>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::resolve<AST_ITYPE_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_itypes>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::resolve<AST_NAME_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_names>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::resolve<AST_CHILD_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_children>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::resolve<AST_ARRAY_ELEMENT_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_arr_elements>& get(NodeComponentRegistry& reg);
};

template<>
struct NodeComponentRegistry::resolve<AST_NODE_RETURN_COMPONENT> {
    PRYSMA_NODISCARD static sparse_set<of_return_nodes>& get(NodeComponentRegistry& reg);
};


template<typename COMPONENT_TAG, typename T>
auto NodeComponentRegistry::insert(node_id_t id, T&& component) noexcept -> void {
    resolve<COMPONENT_TAG>::get(*this).insert(id, std::forward<T>(component));
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::emplace(node_id_t id) noexcept -> void {
    resolve<COMPONENT_TAG>::get(*this).emplace(id);
}

template<typename COMPONENT_TAG>
auto NodeComponentRegistry::get(node_id_t id) noexcept -> auto& {
    auto* ptr = resolve<COMPONENT_TAG>::get(*this).get(id);

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
    resolve<COMPONENT_TAG>::get(*this).remove(id);
}