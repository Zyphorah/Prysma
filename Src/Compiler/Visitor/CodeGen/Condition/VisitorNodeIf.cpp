#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/Visitor/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeIf* nodeIf) 
{
    // Récupérer la condition et les blocs à partir du node if
    INode* nodeCondition = nodeIf->getNodeCondition();
    INode* nodeBlocIf = nodeIf->getNodeBlocIf();
    INode* nodeBlocElse = nodeIf->getNodeBlocElse();

    // Évaluer la condition
    nodeCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->getValeurTemporaire().getAdresse(); 

    llvm::Function* functionEnCours = _contextGenCode->getBackend()->getBuilder().GetInsertBlock()->getParent();

    auto [blocThen, blocElse, blocFin] = ControlFlowHelper::creerBlocsControle(functionEnCours, _contextGenCode->getBackend()->getContext(), "if", "else", "endif");

    // Générer le code de branchement conditionnel
    _contextGenCode->getBackend()->getBuilder().CreateCondBr(cmp, blocThen, blocElse);

    // Générer le code pour le bloc "if"
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocThen);
    if (nodeBlocIf != nullptr) {
        nodeBlocIf->accept(this);
    }
    // Brancher vers endif à la fin du bloc if
    _contextGenCode->getBackend()->getBuilder().CreateBr(blocFin);

    // Générer le code pour le bloc "else"
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocElse);
    if (nodeBlocElse != nullptr) {
        nodeBlocElse->accept(this);
    }
    // Brancher vers endif à la fin du bloc else
    _contextGenCode->getBackend()->getBuilder().CreateBr(blocFin);

    // Générer le code pour le bloc "endif"
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocFin);
    
    // Car plus de node devrait traiter cette valeur temporaire après le if
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));

}
