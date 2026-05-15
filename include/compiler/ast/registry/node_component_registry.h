#pragma once

#include <cstddef>
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

// Les données sont immuables et accessibles sans getter. C'est réfléchit
// et la décision est logique, elle améliore la lisibilité et l'extensibilité.

// LISTE DES COMPONSANTES DU SYSTÈME
struct NodeInstructionComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Instruction; // SUS
    llvm::ArrayRef<INode*> children;

public:
    explicit NodeInstructionComponents(llvm::ArrayRef<INode*> p_children)
        : children(p_children) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getChildren() { return children; }
};

struct NodeCallFunctionComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::CallFunction;
    Token nomFunction;
    llvm::ArrayRef<INode*> children;

public:
    NodeCallFunctionComponents(Token p_nomFunction, llvm::ArrayRef<INode*> p_children)
        : nomFunction(p_nomFunction), children(p_children) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomFunction; }
    llvm::ArrayRef<INode*> getChildren() { return children; }
};

struct NodeArgFunctionComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::ArgFunction;
    IType* type;
    Token nom;

public:
    NodeArgFunctionComponents(IType* p_type, Token p_nom)
        : type(p_type), nom(p_nom) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    IType* getType() { return type; }
    Token getName() { return nom; }
};

struct NodeDeclarationFunctionComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::DeclarationFunction;
    Token visibilite;
    IType* typeReturn;
    Token nom;
    llvm::ArrayRef<INode*> arguments;
    INode* body;

public:
    NodeDeclarationFunctionComponents(Token p_visibilite, IType* p_typeReturn, Token p_nom,
                                      llvm::ArrayRef<INode*> p_arguments, INode* p_body)
        : visibilite(p_visibilite),
          typeReturn(p_typeReturn),
          nom(p_nom),
          arguments(p_arguments),
          body(p_body) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getVisibility() { return visibilite; }
    IType* getReturnType() { return typeReturn; }
    Token getName() { return nom; }
    llvm::ArrayRef<INode*> getArguments() { return arguments; }
    INode* getBody() { return body; }
};

struct NodeReturnComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Return;
    INode* valeurReturn;

public:
    NodeReturnComponents(INode* p_valeurReturn)
        : valeurReturn(p_valeurReturn) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getReturnValue() { return valeurReturn; }
};

struct NodeAssignmentVariableComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::AssignmentVariable;
    Token nom;
    INode* expression;
    Token token;

public:
    NodeAssignmentVariableComponents(Token p_nom, INode* p_expression, Token p_token)
        : nom(p_nom), expression(p_expression), token(p_token) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nom; }
    INode* getExpression() { return expression; }
    Token getToken() { return token; }
};

struct NodeDeclarationVariableComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::DeclarationVariable;
    Token visibilite;
    Token nom;
    IType* type;
    INode* expression;

public:
    NodeDeclarationVariableComponents(Token p_visibilite, Token p_nom, IType* p_type, INode* p_expression)
        : visibilite(p_visibilite), nom(p_nom), type(p_type), expression(p_expression) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getVisibility() { return visibilite; }
    Token getName() { return nom; }
    IType* getType() { return type; }
    INode* getExpression() { return expression; }
};

struct NodeRefVariableComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::RefVariable;
    Token nomVariable;

public:
    NodeRefVariableComponents(Token p_nomVariable)
        : nomVariable(p_nomVariable) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomVariable; }
};

struct NodeUnRefVariableComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::UnRefVariable;
    Token nomVariable;

public:
    NodeUnRefVariableComponents(Token p_nomVariable)
        : nomVariable(p_nomVariable) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomVariable; }
};

struct NodeIdentifiantComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Identifiant;

public:
    NodeIdentifiantComponents() = default;

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
};

struct NodeAssignmentArrayComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::AssignmentArray;
    Token nom;
    INode* expressionIndex;
    INode* expression;
    Token token;

public:
    NodeAssignmentArrayComponents(Token p_nom, INode* p_expressionIndex,
                                  INode* p_expression, Token p_token)
        : nom(p_nom),
          expressionIndex(p_expressionIndex),
          expression(p_expression),
          token(p_token) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nom; }
    INode* getExpressionIndex() { return expressionIndex; }
    INode* getExpression() { return expression; }
    Token getToken() { return token; }
};

struct NodeArrayInitializationComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::ArrayInitialization;
    llvm::ArrayRef<INode*> elements;

public:
    NodeArrayInitializationComponents(llvm::ArrayRef<INode*> p_elements)
        : elements(p_elements) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getElements() { return elements; }
};

struct NodeClassComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Class;

    llvm::ArrayRef<INode*> heritage;
    llvm::ArrayRef<INode*> listMembers;
    llvm::ArrayRef<INode*> builder;
    Token nomClass;

public:
    NodeClassComponents(llvm::ArrayRef<INode*> p_heritage,
                        llvm::ArrayRef<INode*> p_listMembers,
                        llvm::ArrayRef<INode*> p_builder,
                        Token p_nomClass)
        : heritage(p_heritage),
          listMembers(p_listMembers),
          builder(p_builder),
          nomClass(p_nomClass) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getInheritance() { return heritage; }
    llvm::ArrayRef<INode*> getMembers() { return listMembers; }
    llvm::ArrayRef<INode*> getBuilder() { return builder; }
    Token getName() { return nomClass; }
};

struct NodeReadingArrayComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::ReadingArray;
    INode* indexEquation;
    Token nomArray;

public:
    NodeReadingArrayComponents(INode* p_indexEquation, Token p_nomArray)
        : indexEquation(p_indexEquation), nomArray(p_nomArray) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getIndexEquation() { return indexEquation; }
    Token getName() { return nomArray; }
};

struct NodeCallObjectComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::CallObject;
    Token nomObject;
    Token nomMethode;
    llvm::ArrayRef<INode*> children;

public:
    NodeCallObjectComponents(Token p_nomObject, Token p_nomMethode,
                             llvm::ArrayRef<INode*> p_children)
        : nomObject(p_nomObject),
          nomMethode(p_nomMethode),
          children(p_children) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getObjectName() { return nomObject; }
    Token getMethodName() { return nomMethode; }
    llvm::ArrayRef<INode*> getChildren() { return children; }
};

struct NodeAccesAttributeComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::AccesAttribute;
    Token nomObject;
    Token nomAttribute;

public:
    NodeAccesAttributeComponents(Token p_nomObject, Token p_nomAttribute)
        : nomObject(p_nomObject), nomAttribute(p_nomAttribute) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getObjectName() { return nomObject; }
    Token getAttributeName() { return nomAttribute; }
};

struct NodeDeclarationObjectComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::DeclarationObject;
    Token nomObject;
    IType* typeObject;

public:
    NodeDeclarationObjectComponents(Token p_nomObject, IType* p_typeObject)
        : nomObject(p_nomObject), typeObject(p_typeObject) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getObjectName() { return nomObject; }
    IType* getObjectType() { return typeObject; }
};

struct NodeIfComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::If;
    INode* nodeCondition;
    INode* nodeBlocIf;
    INode* nodeBlocElse;
    INode* nodeBlocEndif;

public:
    NodeIfComponents(INode* p_nodeCondition, INode* p_nodeBlocIf,
                     INode* p_nodeBlocElse, INode* p_nodeBlocEndif)
        : nodeCondition(p_nodeCondition),
          nodeBlocIf(p_nodeBlocIf),
          nodeBlocElse(p_nodeBlocElse),
          nodeBlocEndif(p_nodeBlocEndif) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getNodeCondition() { return nodeCondition; }
    INode* getNodeBlocIf() { return nodeBlocIf; }
    INode* getNodeBlocElse() { return nodeBlocElse; }
    INode* getNodeBlocEndif() { return nodeBlocEndif; }
};

struct NodeNewComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::New;
    llvm::ArrayRef<INode*> arguments;
    Token nomType;

public:
    NodeNewComponents(llvm::ArrayRef<INode*> p_arguments, Token p_nomType)
        : arguments(p_arguments), nomType(p_nomType) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getArguments() { return arguments; }
    Token getName() { return nomType; }
};

struct NodeDeleteComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Delete;
    Token nomType;

public:
    NodeDeleteComponents(Token p_nomType)
        : nomType(p_nomType) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomType; }
};

struct NodeIncludeComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Include;
    Token path;

public:
    NodeIncludeComponents(Token p_path)
        : path(p_path) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getPath() { return path; }
};

struct NodeWhileComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::While;
    INode* nodeCondition;
    INode* nodeBlocWhile;
    INode* nodeBlocFinWhile;

public:
    NodeWhileComponents(INode* p_nodeCondition, INode* p_nodeBlocWhile,
                        INode* p_nodeBlocFinWhile)
        : nodeCondition(p_nodeCondition),
          nodeBlocWhile(p_nodeBlocWhile),
          nodeBlocFinWhile(p_nodeBlocFinWhile) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getNodeCondition() { return nodeCondition; }
    INode* getNodeWhileBlock() { return nodeBlocWhile; }
    INode* getNodeWhileEndBlock() { return nodeBlocFinWhile; }
};

struct NodeOperationComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Operation;
    Token token;
    INode* gauche;
    INode* droite;

public:
    NodeOperationComponents(Token p_token, INode* p_gauche, INode* p_droite)
        : token(p_token), gauche(p_gauche), droite(p_droite) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getToken() { return token; }
    INode* getLeft() { return gauche; }
    INode* getRight() { return droite; }
};

struct NodeLiteralComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Literal;
    Token token;

public:
    NodeLiteralComponents(Token p_token)
        : token(p_token) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getToken() { return token; }
};

struct NodeNegationComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Negation;
    Token operateur;
    INode* operande;

public:
    NodeNegationComponents(Token p_operateur, INode* p_operande)
        : operateur(p_operateur), operande(p_operande) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getOperator() { return operateur; }
    INode* getOperand() { return operande; }
};

struct NodeStringComponents {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::String;

public:
    NodeStringComponents() = default;

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
};

// FIN DE LA SECTION

/************************************************************************/



// // namespace prysma::rtag {
// //     struct source {};

// //     struct child {};
// //     struct name {};

// //     struct condition_node {};

// //     struct return_node {};

// //     //struct node_return {};
// //     struct node_gen_type {};
// //     struct itype {};
// //     struct array_element {};
// // }


// struct AST_NODEGEN_TYPE_COMPONENT;
// struct AST_ITYPE_COMPONENT;
// struct AST_NAME_COMPONENT;

// struct AST_CHILD_COMPONENT;         // POUR SÉPARER LES ELEMENTS DES ENFANTS DU SYSTÈME
// struct AST_ARRAY_ELEMENT_COMPONENT; // AFIN DE PERMETTRE ITÉRER SUR L'ARRAY DES CHILD DU SYSTÈME PAR EX

// struct AST_NODE_RETURN_COMPONENT;


// /************************************************************************/


// using of_nodegen_types = NodeTypeGenerated;
// using of_itypes = IType*; // sont alloc dans le pool de bump alloc donc c'est good

// using of_names = Token;
// using of_children = llvm::ArrayRef<INode*>; // les enfants sont alloc dans pool de bump alloc
// using of_arr_elements = llvm::ArrayRef<INode*>; 

// using of_return_nodes = INode*;


// /************************************************************************/


// #if 0
// using of_NodeTypeGenerated = NodeTypeGenerated;
// using of_IType = IType*;

// using of_Token = Token;
// #endif


/************************************************************************/

// À GÉNÉRER AVEC JINJA2

// TODO: éliminer les NodeTypeGenerated et IsA<T, U> en faveur d'un approche reliant sur les types
// TODO: changer les std::size_t en std::size_t& dans les modules concernés

using NodeComponentStorage = std::tuple<
    sparse_set<NodeInstructionComponents>,
    sparse_set<NodeCallFunctionComponents>,
    sparse_set<NodeArgFunctionComponents>,
    sparse_set<NodeDeclarationFunctionComponents>,
    sparse_set<NodeReturnComponents>,
    sparse_set<NodeAssignmentVariableComponents>,
    sparse_set<NodeDeclarationVariableComponents>,
    sparse_set<NodeRefVariableComponents>,
    sparse_set<NodeUnRefVariableComponents>,
    sparse_set<NodeIdentifiantComponents>,
    sparse_set<NodeAssignmentArrayComponents>,
    sparse_set<NodeArrayInitializationComponents>,
    sparse_set<NodeReadingArrayComponents>,
    sparse_set<NodeClassComponents>,
    sparse_set<NodeCallObjectComponents>,
    sparse_set<NodeAccesAttributeComponents>,
    sparse_set<NodeDeclarationObjectComponents>,
    sparse_set<NodeIfComponents>,
    sparse_set<NodeNewComponents>,
    sparse_set<NodeDeleteComponents>,
    sparse_set<NodeIncludeComponents>,
    sparse_set<NodeWhileComponents>,
    sparse_set<NodeOperationComponents>,
    sparse_set<NodeLiteralComponents>,
    sparse_set<NodeNegationComponents>,
    sparse_set<NodeStringComponents>
>;


/************************************************************************/


struct NodeComponentRegistry {
    NodeComponentRegistry() = default;

public:
    template<typename StoredType> struct resolve;

public:
    template<typename StoredType, typename T>
    auto insert(node_id_t id, T&& component) noexcept -> void;

    template<typename StoredType, typename... Args>
    auto emplace(node_id_t id, Args&&... args) noexcept -> void;

    template<typename StoredType>
    auto get(node_id_t id) noexcept -> auto&;

    template<typename StoredType>
    auto remove(node_id_t id) noexcept -> void;

public:
    auto getNextId() noexcept -> node_id_t;

private:
    node_id_t globalId = 0; 
    // chaques threads ont leur compteur, deux registres de deux
    // threads peuvent avoir les mêmes ids mais qui sont destinés 
    // à des éléments différents. dans le cas ou il faudrait que les
    // id soient compatibles entre les différents threads, il faudrait
    // transformer le currentId en membre statique et s'assurer qu'il
    // est thread safe afin d'éviter les race ou les incrémentations invalides

private:
    NodeComponentStorage storage_; // FAIRE RESERVE UN MOMENT DONNÉ

};


/************************************************************************/


template<typename StoredType>
struct NodeComponentRegistry::resolve {
    PRYSMA_NODISCARD static sparse_set<StoredType>& on(NodeComponentRegistry& reg);
};


template<typename StoredType, typename T>
auto NodeComponentRegistry::insert(node_id_t id, T&& component) noexcept -> void {
    resolve<StoredType>::on(*this).insert(id, std::forward<T>(component));
}

template<typename StoredType, typename... Args>
auto NodeComponentRegistry::emplace(node_id_t id, Args&&... args) noexcept -> void {
    resolve<StoredType>::on(*this).emplace(id, std::forward<Args>(args)...);
}

template<typename StoredType>
PRYSMA_NODISCARD auto NodeComponentRegistry::get(std::size_t id) noexcept -> auto& {
    auto* ptr = resolve<StoredType>::on(*this).get(id);

    if (ptr) PRYSMA_LIKELY_BRANCH return *ptr;
    throw std::out_of_range("AST component not found");
}

template<typename StoredType>
auto NodeComponentRegistry::remove(node_id_t id) noexcept -> void {
    resolve<StoredType>::on(*this).remove(id);
}










// template<typename COMPONENT_TAG>
// auto NodeComponentRegistry::get(node_id_t id) noexcept -> auto& {
//     auto* ptr = resolve<COMPONENT_TAG>::get(*this).get(id);

//     if (ptr) PRYSMA_LIKELY_BRANCH 
//         return *ptr; // ou std::optional tout dépendamment ce que j'opterai
//     throw std::out_of_range("AST component not found"); // crash manuel

//     // la logique est parfait mais c'est surtout de savoir si le sparse set
//     // return un ptr ou une ref.
    
//     // C'EST UN PATCH TEMPORAIRE EN ATTENDANT QUE LE SPARSE SET SOIT STABLE
//     // ON RETOURNE UNE RÉFÉRENCE VERS LA RESSOURCE POINTÉ. DANS L'IDÉAL, IL 
//     // FAUDRAIT PROBABLEMENT QUE LE SPARSE SET RETOURNE UNE RÉFÉRENCE AU LIEU
//     // D'UN POINTEUR. CE N'EST PAS CLAIR, IL FAUDRAIT EN DISCUTER. 
//     //return mapper<COMPONENT_TAG>::get(*this).get(id);
// }
