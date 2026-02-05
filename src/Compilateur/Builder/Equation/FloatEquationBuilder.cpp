#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/AST/Noeuds/Interfaces/IExpression.h"

llvm::Value* FloatEquationBuilder::chargerVariable(llvm::Value* adresseMemoire, llvm::Type* type, const std::string& nomVariable)
{
    return _backend->getBuilder().CreateLoad(type, adresseMemoire, nomVariable);
}

void FloatEquationBuilder::construireRegistreSymboleFloat()
 {
           // registre de lambda LLVM contenant une map des opérations mathématiques de base 
        registreSymboleFloat->enregistrer(TOKEN_PLUS, [&]() -> std::shared_ptr<IExpression> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([backend = _backend](llvm::Value* lhs, llvm::Value* rhs) { 
                    llvm::Type* floatType = llvm::Type::getFloatTy(backend->getContext());
                    lhs = backend->creerAutoCast(lhs, floatType);
                    rhs = backend->creerAutoCast(rhs, floatType);
                    return backend->getBuilder().CreateFAdd(lhs, rhs, "addtmp");
                })
            ); 
        });
        
        registreSymboleFloat->enregistrer(TOKEN_MOINS, [&]() -> std::shared_ptr<IExpression> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([backend = _backend](llvm::Value* lhs, llvm::Value* rhs) { 
                    llvm::Type* floatType = llvm::Type::getFloatTy(backend->getContext());
                    lhs = backend->creerAutoCast(lhs, floatType);
                    rhs = backend->creerAutoCast(rhs, floatType);
                    return backend->getBuilder().CreateFSub(lhs, rhs, "subtmp"); 
                })
            ); 
        });
        
        registreSymboleFloat->enregistrer(TOKEN_ETOILE, [&]() -> std::shared_ptr<IExpression> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([backend = _backend](llvm::Value* lhs, llvm::Value* rhs) { 
                    llvm::Type* floatType = llvm::Type::getFloatTy(backend->getContext());
                    lhs = backend->creerAutoCast(lhs, floatType);
                    rhs = backend->creerAutoCast(rhs, floatType);
                    return backend->getBuilder().CreateFMul(lhs, rhs, "multmp"); 
                })
            ); 
        });
        
        registreSymboleFloat->enregistrer(TOKEN_SLASH, [&]() -> std::shared_ptr<IExpression> { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([backend = _backend](llvm::Value* lhs, llvm::Value* rhs) { 
                    llvm::Type* floatType = llvm::Type::getFloatTy(backend->getContext());
                    lhs = backend->creerAutoCast(lhs, floatType);
                    rhs = backend->creerAutoCast(rhs, floatType);
                    return backend->getBuilder().CreateFDiv(lhs, rhs, "divtmp"); 
                })
            ); 
        });
 }      

shared_ptr<INoeud> FloatEquationBuilder::builderArbreEquationFloat(vector<Token> &tokens)
{
    return constructeurArbreEquation->construire(tokens);   
}
