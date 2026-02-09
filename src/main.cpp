#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/LLVM/LlvmSerializer.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>
#include "Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

int main(int argc, char* argv[])
{
    try {

        std::shared_ptr<LlvmBackend> backend = std::make_shared<LlvmBackend>();
        std::shared_ptr<RegistreInstruction> registreInstruction = std::make_shared<RegistreInstruction>();
        std::shared_ptr<RegistreVariable> registreVariable = std::make_shared<RegistreVariable>();
        std::shared_ptr<RegistreFonction> registreFonction = std::make_shared<RegistreFonction>();
        std::shared_ptr<RegistreType> registreType = std::make_shared<RegistreType>();
        std::shared_ptr<RetourContexteCompilation> returnContextCompilation = std::make_shared<RetourContexteCompilation>();
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
        context->registreFonction->enregistrer("printInt", context->backend->getModule().getFunction("printInt"));

        // PrintFloat 
        IntegerType* floatTy = llvm::Type::getInt32Ty(context->backend->getContext());
        std::vector<llvm::Type*> printFloatArgs = {floatTy};
        context->backend->declarerExterne("printFloat", floatTy, printFloatArgs);
        context->registreFonction->enregistrer("printFloat", context->backend->getModule().getFunction("printFloat"));
        
        // Enregistrer les types de base
        context->registreType->enregistrer(TOKEN_TYPE_INT, llvm::Type::getInt32Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(context->backend->getContext()));
        
        // NoeudInstruction du langage prysma
        context->registreInstruction->enregistrer(TOKEN_SCOPE, std::make_shared<ParseurScope>());
        context->registreInstruction->enregistrer(TOKEN_FONCTION, std::make_shared<ParseurDeclarationFonction>());
        context->registreInstruction->enregistrer(TOKEN_AFF, std::make_shared<ParseurAffectationVariable>(context->backend, context->registreVariable, context->registreType));
        context->registreInstruction->enregistrer(TOKEN_DEC, std::make_shared<ParseurDeclarationVariable>());
        context->registreInstruction->enregistrer(TOKEN_CALL, std::make_shared<ParseurAppelFonction>());
        context->registreInstruction->enregistrer(TOKEN_RETOUR, std::make_shared<ParseurRetour>());
        context->registreInstruction->enregistrer(TOKEN_ARG, std::make_shared<ParseurArgFonction>());
        context->registreInstruction->enregistrer(TOKEN_UNREF, std::make_shared<ParseurUnRefVariable>());
        context->registreInstruction->enregistrer(TOKEN_REF, std::make_shared<ParseurRefVariable>());

        ConstructeurArbreInstruction constructeurArbreInstruction(context->registreInstruction);
        std::shared_ptr<INoeud> arbre = constructeurArbreInstruction.construire(tokens);

        VisiteurGeneralGenCode visiteur(context);
        arbre->accept(&visiteur);

        LlvmSerializer serializer(context->backend->getContext(), context->backend->getModule());
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