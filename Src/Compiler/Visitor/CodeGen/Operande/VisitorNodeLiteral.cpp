#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <cstdint>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <string>

void GeneralVisitorGenCode::visiter(NodeLiteral* nodeLiteral)
{
    llvm::LLVMContext& context = _contextGenCode->getBackend()->getContext();
    Token token = nodeLiteral->getToken();

    if (token.type == TOKEN_IDENTIFIANT) {
        LoadurVariable loadur(_contextGenCode);
        _contextGenCode->modifierValeurTemporaire( loadur.loadr(token.value));
        return; 
    }

    llvm::Value* llvmValue = nullptr;
    llvm::Type* llvmType = nullptr;

    if (token.type == TOKEN_VRAI || token.type == TOKEN_FAUX) {
        bool valeur = (token.type == TOKEN_VRAI);
        llvmType = llvm::Type::getInt1Ty(context);
        llvmValue = llvm::ConstantInt::get(llvmType, valeur ? 1 : 0);
    }
    else if (token.type == TOKEN_LIT_BOLEEN) {
        int valeur = std::stoi(token.value);
        llvmType = llvm::Type::getInt1Ty(context);
        llvmValue = llvm::ConstantInt::get(llvmType, valeur != 0 ? 1 : 0);
    }
    else if (token.type == TOKEN_LIT_INT) { 
        int valeur = std::stoi(token.value); 
        llvmType = llvm::Type::getInt32Ty(context);
        llvmValue = llvm::ConstantInt::get(llvmType, static_cast<uint64_t>(valeur), true);
    }
    else if (token.type == TOKEN_LIT_FLOAT) { 
        float valeur = std::stof(token.value); 
        llvmType = llvm::Type::getFloatTy(context);
        llvmValue = llvm::ConstantFP::get(llvmType, valeur);
    }
    else {
        ErrorHelper::errorCompilation("Type de littéral non supporté (" + token.value + ")");
    }

    _contextGenCode->modifierValeurTemporaire(Symbole(llvmValue, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(llvmType), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}