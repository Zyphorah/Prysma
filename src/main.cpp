#include "Compilateur/AST/Noeuds/Variable/registreVariable.h"

// Inclusions LLVM
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include "Compilateur/LLVM/LLVMSerializer.h"

#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"

#include <iostream>
#include "Compilateur/TraitementFichier/FichierLecture.h"

// Target machine cible
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <memory>

int main() {
    try {
        // Utilisation de shared_ptr 
        // Instruction

        // Faire un registre de lamda qui contient un dictionnaire mapé construit dynamiquement pour L'AST contenant un dictionnaire de noeud relier à des lamdas
        // But encapsuler la logique de récursivité similaire pour l'arbre syntaxique abstrait, plus besoins de ce concentrer sur la récursivité car elle est similaire pour tout les cas
        // Utilisation d'un système Table de Dispatch, rend le code ultra extensible pour toute nouveauté future

        /*
            IExpression.h
            class IExpression {
            public:
                virtual ~IExpression() = default;
                // La méthode (Génération de code)
                virtual llvm::Value* codeGen(Context& ctx) = 0; 
            };

                        // NoeudInstruction.h
            class NoeudInstruction : public IExpression {
            private:
                // La "recette" pour générer le code LLVM de cette istd :: cerr << "Impossible d'ouvrir le fichier !";
    return "";nstruction spécifique
                // Elle capture tout ce dont elle a besoin (condition, corps...)
                std::function<llvm::Value*()> _generateurCode;

            public:
                NoeudInstruction(std::function<llvm::Value*()> generateur) 
                    : _generateurCode(generateur) {}

                llvm::Value* genererCode() override {
                    // On exécute simplement la lambda stockée
                    return _generateurCode();
                }
        };
        */

          // ===== Initialisation LLVM =====
        llvm::LLVMContext context;
        llvm::Module module("PrysmaModule", context);
        llvm::IRBuilder<llvm::NoFolder> builder(context);
    

        // ===== Configuration de la cible, évite d'avoir des adresses mémoire aléatoire, ce qui cause des crashs aléatoire. =====
        InitializeAllTargetInfos();
        InitializeAllTargets();
        InitializeAllTargetMCs();
        InitializeAllAsmPrinters();

        std::string targetTriple = sys::getDefaultTargetTriple();
        module.setTargetTriple(targetTriple);

        std::string error;
        auto target = TargetRegistry::lookupTarget(targetTriple, error);
        if (!target) {
            errs() << error;
            return 1;
        }

        TargetOptions opt;
        auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, Reloc::Model::PIC_);
        
        // == teste du registre == 

        std::shared_ptr<RegistreVariable> registreVariable = std::make_shared<RegistreVariable>();

        
        // Création fonction main qui retourne un double
        llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
        llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", &module);
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
        builder.SetInsertPoint(entry);

        // Test simple du registre - créer une allocation avec IRBuilder
        Token testToken{TOKEN_IDENTIFIANT, "testVar"};
        llvm::AllocaInst* allocaRaw = builder.CreateAlloca(builder.getDoubleTy(), nullptr, "test");
        std::shared_ptr<llvm::AllocaInst> allocaPtr = std::shared_ptr<llvm::AllocaInst>(allocaRaw, [](llvm::AllocaInst*){});
        registreVariable->enregistrer(testToken, allocaPtr);
        std::shared_ptr<llvm::AllocaInst> resultTest = registreVariable->recupererVariables(testToken);

        // Rechercher dans le dictionnaire 
        std::shared_ptr<llvm::AllocaInst> allocation = registreVariable->recupererVariables(testToken);

        // ===== Configuration du registre =====
        std::string document;
        FichierLecture fichierLecture("../src/PrysmaCodeTests/main.prysma");
        document = fichierLecture.entrer();
  
        Lexer lexer; 
        
        vector<Token> tokens = lexer.tokenizer(document);

        FloatEquationBuilder* floatEquationBuilder = new FloatEquationBuilder(context);
        
        shared_ptr<INoeud> expression = floatEquationBuilder->builderArbreEquationFloat(tokens);
        
        // Résoudre l'expression (cela appelle aussi les lambdas LLVM) / Parcours en Profondeur Postfixe
        llvm::Value* resultatNumerique = expression->genCode();
             
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
        delete floatEquationBuilder;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}