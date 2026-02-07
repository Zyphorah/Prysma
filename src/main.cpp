#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/ReturnContextCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/LLVM/LLVMSerializer.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>
#include "Compilateur/Parsing/Instruction/ParserScope.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParsingDeclarationFonction.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

int main(int argc, char* argv[])
{
    try {

        std::shared_ptr<LLVMBackend> backend = std::make_shared<LLVMBackend>();
        std::shared_ptr<RegistreInstruction> registreInstruction = std::make_shared<RegistreInstruction>();
        std::shared_ptr<RegistreVariable> registreVariable = std::make_shared<RegistreVariable>();
        std::shared_ptr<RegistreFonction> registreFonction = std::make_shared<RegistreFonction>();
        std::shared_ptr<RegistreType> registreType = std::make_shared<RegistreType>();
        std::shared_ptr<ReturnContextCompilation> returnContextCompilation = std::make_shared<ReturnContextCompilation>();
        std::shared_ptr<RegistreArgument> registreArgument = std::make_shared<RegistreArgument>();
        llvm::Value* valeurTemporaire = nullptr;

        std::shared_ptr<ContextGenCode> context = std::make_shared<ContextGenCode>(
            backend,
            registreInstruction,
            registreVariable,
            registreFonction,
            registreType,
            returnContextCompilation,
            registreArgument,
            valeurTemporaire
        );

        FichierLecture fichierLecture("../src/PrysmaCodeTests/main.p");
        std::string document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

        // Enregistrer des fonctions externes

        // PrintInt
        IntegerType* intTy = llvm::Type::getInt32Ty(context->backend->getContext());
        std::vector<llvm::Type*> printIntArgs = {intTy};
        context->backend->declarerExterne("printInt", intTy, printIntArgs);
        context->registreFonction->ajouter("printInt", context->backend->getModule().getFunction("printInt"));

        // PrintFloat 
        IntegerType* floatTy = llvm::Type::getInt32Ty(context->backend->getContext());
        std::vector<llvm::Type*> printFloatArgs = {floatTy};
        context->backend->declarerExterne("printFloat", floatTy, printFloatArgs);
        context->registreFonction->ajouter("printFloat", context->backend->getModule().getFunction("printFloat"));
        
        // Enregistrer les types de base
        context->registreType->enregistrer(TOKEN_TYPE_INT, llvm::Type::getInt32Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(context->backend->getContext()));
        
        // NoeudInstruction du langage prysma
        context->registreInstruction->enregistrer(TOKEN_SCOPE, std::make_shared<ParserScope>());
        //context->registreInstruction->enregistrer(TOKEN_FONCTION, std::make_shared<ParsingDeclarationFonction>(TOKEN_FONCTION));
        //context->registreInstruction->enregistrer(TOKEN_AFF, std::make_shared<ParseurAffectationVariable>());
        context->registreInstruction->enregistrer(TOKEN_DEC, std::make_shared<ParseurDeclarationVariable>());
        //context->registreInstruction->enregistrer(TOKEN_CALL, std::make_shared<ParserAppelFonction>());
        //context->registreInstruction->enregistrer(TOKEN_RETOUR, std::make_shared<ParsingReturn>());
        //context->registreInstruction->enregistrer(TOKEN_ARG, std::make_shared<ParserArgFonction>());
        //context->registreInstruction->enregistrer(TOKEN_PASS, std::make_shared<ParserArgPassFonction>());

        ConstructeurArbreInstruction constructeurArbreInstruction(context->registreInstruction);
        std::shared_ptr<INoeud> arbre = constructeurArbreInstruction.construire(tokens);

        VisiteurGeneralGenCode visiteur(context);
        arbre->accept(&visiteur);

        LLVMSerializer serializer(context->backend->getContext(), context->backend->getModule());
        serializer.SauvegarderCodeLLVM("output.ll");
       
        ConstructeurSysteme constructeur("../src/Lib", "Lib", "output.ll", "programme");
        constructeur.compilerLib();
        constructeur.lierLibExecutable();
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return -1;
    }
}