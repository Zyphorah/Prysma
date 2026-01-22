#include "Compilateur/AST/Noeuds/Operande/RegistreSymbole.h"
#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include "Compilateur/Parsing/Equation/ChaineResponsabilite.h"
#include "Compilateur/Parsing/Equation/ServiceParenthese.h"
#include "Compilateur/Parsing/Equation/GestionnaireOperateur.h"
#include "Compilateur/AST/Noeuds/Operande/Operation.h"

// Inclusions LLVM
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include "Compilateur/LLVM/LLVMSerializer.h"

#include <iostream>

int main() {
    try {
        // ===== Initialisation LLVM =====
        llvm::LLVMContext context;
        llvm::Module module("PrysmaModule", context);
        llvm::IRBuilder<llvm::NoFolder> builder(context);
    
        // Création fonction main qui retourne un double
        llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
        llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", &module);
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
        builder.SetInsertPoint(entry);

        // ===== Configuration du registre =====
        std::string equation = "2*3-(20+3)";
        std::shared_ptr<RegistreSymbole> registreSymbole = std::make_shared<RegistreSymbole>();

        // registre de lamda LLVM contenant une map des opérations mathématiques de base 
        registreSymbole->enregistrer('+', [&]() { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFAdd(lhs, rhs, "addtmp"); 
                })
            ); 
        });
        
        registreSymbole->enregistrer('-', [&]() { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFSub(lhs, rhs, "subtmp"); 
                })
            ); 
        });
        
        registreSymbole->enregistrer('*', [&]() { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFMul(lhs, rhs, "multmp"); 
                })
            ); 
        });
        
        registreSymbole->enregistrer('/', [&]() { 
            return std::make_shared<Operation>(
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)>([&](llvm::Value* lhs, llvm::Value* rhs) { 
                    return builder.CreateFDiv(lhs, rhs, "divtmp"); 
                })
            ); 
        });
        
        // ===== Chaîne de responsabilité =====
        ServiceParenthese* serviceParenthese = new ServiceParenthese(registreSymbole);
        
        GestionnaireOperateur* gestionnaireAddition = new GestionnaireOperateur('+');
        GestionnaireOperateur* gestionnaireSoustraction = new GestionnaireOperateur('-');
        GestionnaireOperateur* gestionnaireMultiplication = new GestionnaireOperateur('*');
        GestionnaireOperateur* gestionnaireDivision = new GestionnaireOperateur('/');
        
        std::vector<GestionnaireOperateur*> operateurs = {
            gestionnaireAddition, gestionnaireSoustraction, 
            gestionnaireMultiplication, gestionnaireDivision
        };
        
        ChaineResponsabilite* chaineResponsabilite = new ChaineResponsabilite(serviceParenthese, operateurs);
                        
        // ===== Construction de l'AST et Résolution =====
        ConstructeurArbreEquation* constructeurArbreEquation = new ConstructeurArbreEquation(
            chaineResponsabilite, registreSymbole, serviceParenthese, context
        );

        std::shared_ptr<IExpression> expression = constructeurArbreEquation->construire(equation);
        
        // Résoudre l'expression (cela appelle aussi les lambdas LLVM)
        llvm::Value* resultatNumerique = expression->resoudre();
             
        // ===== Affichage du résultat avec printf =====
        // Déclaration de la fonction printf
        llvm::FunctionType *printfType = llvm::FunctionType::get(
            builder.getInt32Ty(),
            {llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context))},
            true  // variadic
        );
        llvm::FunctionCallee printfFunc = module.getOrInsertFunction("printf", printfType);
        
        // Format string "Résultat: %f\n"
        std::string formatStr = "Resultat: %f\n";
        llvm::Constant *formatConst = llvm::ConstantDataArray::getString(context, formatStr);
        llvm::GlobalVariable *formatGlobal = new llvm::GlobalVariable(
            module,
            formatConst->getType(),
            true,  // isConstant
            llvm::GlobalValue::PrivateLinkage,
            formatConst,
            ".str"
        );
        
        llvm::Value *formatPtr = builder.CreateGEP(
            formatGlobal->getValueType(),
            formatGlobal,
            {builder.getInt64(0), builder.getInt64(0)},
            "formatPtr"
        );
        
        // Appel à printf avec le résultat
        std::vector<llvm::Value*> printfArgs = {formatPtr, resultatNumerique};
        builder.CreateCall(printfFunc, printfArgs);
        
        // Retourner le résultat converti en int32
        llvm::Value* resultInt = builder.CreateFPToSI(resultatNumerique, builder.getInt32Ty(), "resultInt");
        builder.CreateRet(resultInt);

        // ===== Sauvegarde du code LLVM =====

        LLVMSerializer traitementFichier(context, module);
        traitementFichier.SauvegarderCodeLLVM("output.ll");

        // Nettoyage
        delete constructeurArbreEquation;
        delete chaineResponsabilite;
        delete gestionnaireAddition; delete gestionnaireSoustraction;
        delete gestionnaireMultiplication; delete gestionnaireDivision;
        delete serviceParenthese;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}