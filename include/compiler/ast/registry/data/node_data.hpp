//===-- node_data.hpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"
#include <llvm-18/llvm/ADT/ArrayRef.h>
#include "compiler/ast/registry/node_component_registry.h" // UNIQUEMENT TEMPORAIRE ET POUR ÉVITER LE CONFLIT AVEC LA DEF DE NodeTypeGenerated



// enum class NodeTypeGenerated { // TODO: à réactiver du moment que la migration est terminée.
//     Instruction,
//     CallFunction,
//     ArgFunction,
//     DeclarationFunction,
//     Return,
//     AssignmentVariable,
//     DeclarationVariable,
//     RefVariable,
//     UnRefVariable,
//     Identifiant,
//     AssignmentArray,
//     ArrayInitialization,
//     ReadingArray,
//     Class,
//     CallObject,
//     AccesAttribute,
//     DeclarationObject,
//     If,
//     New,
//     Delete,
//     Include,
//     While,
//     Operation,
//     Literal,
//     Negation,
//     String
// };

struct InstructionNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Instruction; // SUS
    llvm::ArrayRef<INode*> children;

public:
    InstructionNodeData() = default; 

    explicit InstructionNodeData(llvm::ArrayRef<INode*> p_children = nullptr)
        : children(p_children) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getChildren() { return children; }
};

struct FunctionCallNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::CallFunction;
    Token nomFunction;
    llvm::ArrayRef<INode*> children;

public:
    FunctionCallNodeData(Token p_nomFunction, llvm::ArrayRef<INode*> p_children = nullptr)
        : nomFunction(p_nomFunction), children(p_children) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomFunction; }
    llvm::ArrayRef<INode*> getChildren() { return children; }
};

struct FunctionArgNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::ArgFunction;
    IType* type;
    Token nom;

public:
    FunctionArgNodeData(IType* p_type, Token p_nom)
        : type(p_type), nom(p_nom) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    IType* getType() { return type; }
    Token getName() { return nom; }
};

struct FunctionDeclarationNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::DeclarationFunction;
    Token visibilite;
    IType* typeReturn;
    Token nom;
    llvm::ArrayRef<INode*> arguments;
    INode* body;

public:
    FunctionDeclarationNodeData(Token p_visibilite, IType* p_typeReturn, Token p_nom,
                                llvm::ArrayRef<INode*> p_arguments = nullptr, INode* p_body = nullptr)
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

struct ReturnNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Return;
    INode* valeurReturn;

public:
    ReturnNodeData(INode* p_valeurReturn = nullptr)
        : valeurReturn(p_valeurReturn) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getReturnValue() { return valeurReturn; }
};

struct VariableAssignmentNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::AssignmentVariable;
    Token nom;
    INode* expression;
    Token token;

public:
    VariableAssignmentNodeData(Token p_nom, INode* p_expression, Token p_token)
        : nom(p_nom), expression(p_expression), token(p_token) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nom; }
    INode* getExpression() { return expression; }
    Token getToken() { return token; }
};

struct VariableDeclarationNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::DeclarationVariable;
    Token visibilite;
    Token nom;
    IType* type;
    INode* expression;

public:
    VariableDeclarationNodeData(Token p_visibilite, Token p_nom, IType* p_type = nullptr, INode* p_expression = nullptr)
        : visibilite(p_visibilite), nom(p_nom), type(p_type), expression(p_expression) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getVisibility() { return visibilite; }
    Token getName() { return nom; }
    IType* getType() { return type; }
    INode* getExpression() { return expression; }
};

struct VariableRefNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::RefVariable;
    Token nomVariable;

public:
    VariableRefNodeData(Token p_nomVariable)
        : nomVariable(p_nomVariable) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomVariable; }
};

struct VariableUnrefNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::UnRefVariable;
    Token nomVariable;

public:
    VariableUnrefNodeData(Token p_nomVariable)
        : nomVariable(p_nomVariable) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomVariable; }
};

struct IdentifierNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Identifiant;

public:
    IdentifierNodeData() = default;

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
};

struct ArrayAssignmentNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::AssignmentArray;
    Token nom;
    INode* expressionIndex;
    INode* expression;
    Token token;

public:
    ArrayAssignmentNodeData(Token p_nom, INode* p_expressionIndex,
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

struct ArrayInitializationNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::ArrayInitialization;
    llvm::ArrayRef<INode*> elements;

public:
    ArrayInitializationNodeData(llvm::ArrayRef<INode*> p_elements = nullptr)
        : elements(p_elements) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getElements() { return elements; }
};

struct ClassNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Class;

    llvm::ArrayRef<INode*> heritage;
    llvm::ArrayRef<INode*> listMembers;
    llvm::ArrayRef<INode*> builder;
    Token nomClass;

public:
    ClassNodeData(llvm::ArrayRef<INode*> p_heritage,
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

struct ArrayReadingNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::ReadingArray;
    INode* indexEquation;
    Token nomArray;

public:
    ArrayReadingNodeData(INode* p_indexEquation, Token p_nomArray)
        : indexEquation(p_indexEquation), nomArray(p_nomArray) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getIndexEquation() { return indexEquation; }
    Token getName() { return nomArray; }
};

struct ObjectCallNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::CallObject;
    Token nomObject;
    Token nomMethode;
    llvm::ArrayRef<INode*> children;

public:
    ObjectCallNodeData(Token p_nomObject, Token p_nomMethode,
                       llvm::ArrayRef<INode*> p_children = nullptr)
        : nomObject(p_nomObject),
          nomMethode(p_nomMethode),
          children(p_children) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getObjectName() { return nomObject; }
    Token getMethodName() { return nomMethode; }
    llvm::ArrayRef<INode*> getChildren() { return children; }
};

struct AccessAttributeNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::AccesAttribute;
    Token nomObject;
    Token nomAttribute;

public:
    AccessAttributeNodeData(Token p_nomObject, Token p_nomAttribute)
        : nomObject(p_nomObject), nomAttribute(p_nomAttribute) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getObjectName() { return nomObject; }
    Token getAttributeName() { return nomAttribute; }
};

struct ObjectDeclarationNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::DeclarationObject;
    Token nomObject;
    IType* typeObject;

public:
    ObjectDeclarationNodeData(Token p_nomObject, IType* p_typeObject = nullptr)
        : nomObject(p_nomObject), typeObject(p_typeObject) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getObjectName() { return nomObject; }
    IType* getObjectType() { return typeObject; }
};

struct IfNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::If;
    INode* nodeCondition;
    INode* nodeBlocIf;
    INode* nodeBlocElse;
    INode* nodeBlocEndif;

public:
    IfNodeData(INode* p_nodeCondition = nullptr, INode* p_nodeBlocIf = nullptr,
               INode* p_nodeBlocElse = nullptr, INode* p_nodeBlocEndif = nullptr)
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

struct NewNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::New;
    llvm::ArrayRef<INode*> arguments;
    Token nomType;

public:
    NewNodeData(llvm::ArrayRef<INode*> p_arguments, Token p_nomType)
        : arguments(p_arguments), nomType(p_nomType) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    llvm::ArrayRef<INode*> getArguments() { return arguments; }
    Token getName() { return nomType; }
};

struct DeleteNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Delete;
    Token nomType;

public:
    DeleteNodeData(Token p_nomType)
        : nomType(p_nomType) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getName() { return nomType; }
};

struct IncludeNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Include;
    Token path;

public:
    IncludeNodeData(Token p_path)
        : path(p_path) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getPath() { return path; }
};

struct WhileNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::While;
    INode* nodeCondition;
    INode* nodeBlocWhile;
    INode* nodeBlocFinWhile;

public:
    WhileNodeData(INode* p_nodeCondition = nullptr, INode* p_nodeBlocWhile = nullptr,
                  INode* p_nodeBlocFinWhile = nullptr)
        : nodeCondition(p_nodeCondition),
          nodeBlocWhile(p_nodeBlocWhile),
          nodeBlocFinWhile(p_nodeBlocFinWhile) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    INode* getNodeCondition() { return nodeCondition; }
    INode* getNodeWhileBlock() { return nodeBlocWhile; }
    INode* getNodeWhileEndBlock() { return nodeBlocFinWhile; }
};

struct OperationNodeData { // il est expression je pense
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Operation;
    Token token;
    INode* gauche;
    INode* droite;

public:
    OperationNodeData(Token p_token, INode* p_gauche = nullptr, INode* p_droite = nullptr)
        : token(p_token), gauche(p_gauche), droite(p_droite) {}


    // EXTRÈMEMENT TEMPORAIRE ET ASSEZ UNSAFE, CE N'EST QU'EN ATTENDANT DE TROUVER UNE SOLUTION
    // LE DESIGN NE RESTERA PAS COMME ÇA, IL S'AGIRAIT DE FAIRE CE DONT J'AI ABORDÉ PLUS BAS

    // d'ailleurs, je ne vois pas pourquoi addExpression devrait retourner this? Je pense comprendre que c'est
    // pour un système de chaînage ou quelque chose du genre mais en aucuns cas une structure de donnée ne devrait
    // être dépendante ou altérée par une fonctionnalitée externe comme un builder par exemple. addExpression ne fait
    // qu'altérer les données de l'objet depuis lequel la méthode est appelée, en théorie, nous ne devrions pas retourner
    // this car nous avons littéralement called la méthode depuis cet objet, donc en toute logique, nous y avons déja accès.
    
    // je pourrais me tromper mais pour l'instant, je continue et j'adapte le design.
    void addExpression(INode* left, INode* right) { gauche = left; droite = right; } // devrait être dans genre ExpressionOperationComponents
    // ou peut-être même faire un ExpressionComponentsRegistry et templater le registre que j'ai plus bas pour pouvoir le configurer pour les deux


    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getToken() { return token; }
    INode* getLeft() { return gauche; }
    INode* getRight() { return droite; }
};

// ok, il s'agirait de faire des catégories de contracts. par exemple, avoir un INodeComponent
// ou bien un IExpressionComponent ou bien même avoir des tags sur les noeuds EUX-MÊMES genre
// il hérite de IsExpression ou IsNode. Nous pourrions même faire du crtp afin d'optimiser le tout
// et de profiter d'optimisations suplémentaires (inlining agressif par exemple).


struct LiteralNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Literal;
    Token token;

public:
    LiteralNodeData(Token p_token)
        : token(p_token) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getToken() { return token; }
};

struct NegationNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::Negation;
    Token operateur;
    INode* operande;

public:
    NegationNodeData(Token p_operateur, INode* p_operande = nullptr)
        : operateur(p_operateur), operande(p_operande) {}

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
    Token getOperator() { return operateur; }
    INode* getOperand() { return operande; }
};

struct StringNodeData {
private:
    NodeTypeGenerated nodeTypeGenerated = NodeTypeGenerated::String;

public:
    StringNodeData() = default;

    NodeTypeGenerated getGeneratedNodeType() { return nodeTypeGenerated; }
};