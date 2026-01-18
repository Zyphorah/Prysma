#include "AST/Noeuds/Operande/RegistreSymbole.h"
#include "AST/ConstructeurArbreEquation.h"
#include "Parsing/Equation/ChaineResponsabilite.h"
#include "Parsing/Equation/ServiceParenthese.h"
#include "Parsing/Equation/GestionnaireOperateur.h"
#include "AST/Noeuds/Operande/Operation.h"
#include "AST/Noeuds/Operande/Valeur.h"

// Inclusions LLVM
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <map>

int main() {
    try {
        // ===== Initialisation LLVM =====
        llvm::LLVMContext context;
        llvm::Module module("PrysmaModule", context);
        llvm::IRBuilder<llvm::NoFolder> builder(context);
    
        // Création fonction main qui retourne un double
        llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getDoubleTy(), false);
        llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", &module);
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
        builder.SetInsertPoint(entry);
        
        llvm::Value* resultLLVM = nullptr;

        // ===== Configuration du registre =====
        std::string equation = "3 + 5 * (2 - 8) / 4";
        std::shared_ptr<RegistreSymbole> registreSymbole = std::make_shared<RegistreSymbole>();

        // registre de lamda LLVM contenant une map des opérations mathématiques de base 
        registreSymbole->enregistrer('+', [&]() { 
            return std::make_shared<Operation>([&](double a, double b) { 
                llvm::Value* valA = llvm::ConstantFP::get(context, llvm::APFloat(a));
                llvm::Value* valB = llvm::ConstantFP::get(context, llvm::APFloat(b));
                resultLLVM = builder.CreateFAdd(valA, valB, "addtmp"); 
                return a + b; 
            }); 
        });
        
        registreSymbole->enregistrer('-', [&]() { 
            return std::make_shared<Operation>([&](double a, double b) { 
                llvm::Value* valA = llvm::ConstantFP::get(context, llvm::APFloat(a));
                llvm::Value* valB = llvm::ConstantFP::get(context, llvm::APFloat(b));
                resultLLVM = builder.CreateFSub(valA, valB, "subtmp"); 
                return a - b; 
            }); 
        });
        
        registreSymbole->enregistrer('*', [&]() { 
            return std::make_shared<Operation>([&](double a, double b) { 
                llvm::Value* valA = llvm::ConstantFP::get(context, llvm::APFloat(a));
                llvm::Value* valB = llvm::ConstantFP::get(context, llvm::APFloat(b));
                resultLLVM = builder.CreateFMul(valA, valB, "multmp"); 
                return a * b; 
            }); 
        });
        
        registreSymbole->enregistrer('/', [&]() { 
            return std::make_shared<Operation>([&](double a, double b) { 
                if (b == 0) throw std::runtime_error("Division par zéro");
                llvm::Value* valA = llvm::ConstantFP::get(context, llvm::APFloat(a));
                llvm::Value* valB = llvm::ConstantFP::get(context, llvm::APFloat(b));
                resultLLVM = builder.CreateFDiv(valA, valB, "divtmp"); 
                return a / b; 
            }); 
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
            chaineResponsabilite, registreSymbole, serviceParenthese
        );

        std::shared_ptr<IExpression> expression = constructeurArbreEquation->construire(equation);
        
        // Résoudre l'expression (cela appelle aussi les lambdas LLVM)
        double resultatNumerique = expression->resoudre();
        std::cout << "Résultat numérique: " << resultatNumerique << std::endl;
        
        // Le dernier resultLLVM contient le résultat final
        if (resultLLVM == nullptr) {
            resultLLVM = llvm::ConstantFP::get(context, llvm::APFloat(resultatNumerique));
        }
        
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
        std::vector<llvm::Value*> printfArgs = {formatPtr, resultLLVM};
        builder.CreateCall(printfFunc, printfArgs);
        
        // Retourner le résultat
        builder.CreateRet(resultLLVM);

        // ===== Affichage et Sauvegarde =====
        std::string str;
        llvm::raw_string_ostream os(str);
        module.print(os, nullptr);
        std::cout << "\n--- Code LLVM IR ---" << std::endl;
        std::cout << os.str() << std::endl;
        
        // Sauvegarder le code IR dans un fichier
        std::ofstream irFile("code.ll");
        irFile << os.str();
        irFile.close();
        std::cout << "\n=== Code IR sauvegardé dans code.ll ===" << std::endl;

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