#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"
       
void FloatEquationBuilder::construireRegistreSymboleFloat()
 {
           // registre de lamda LLVM contenant une map des opérations mathématiques de base 
        registreSymboleFloat->enregistrer(TOKEN_PLUS, [&]() -> std::shared_ptr<INoeud> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFAdd(lhs, rhs, "addtmp"); 
                })
            ); 
        });
        
        registreSymboleFloat->enregistrer(TOKEN_MOINS, [&]() -> std::shared_ptr<INoeud> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFSub(lhs, rhs, "subtmp"); 
                })
            ); 
        });
        
        registreSymboleFloat->enregistrer(TOKEN_ETOILE, [&]() -> std::shared_ptr<INoeud> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFMul(lhs, rhs, "multmp"); 
                })
            ); 
        });
        
        registreSymboleFloat->enregistrer(TOKEN_SLASH, [&]() -> std::shared_ptr<INoeud> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFDiv(lhs, rhs, "divtmp"); 
                })
            ); 
        });
 }      

        

shared_ptr<IExpression> FloatEquationBuilder::builderArbreEquationFloat(vector<Token> &tokens)
{
    return constructeurArbreEquation->construire(tokens);   
}
