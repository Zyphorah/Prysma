#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Registre/Pile/ReturnContextCompilation.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/LLVM/LLVMSerializer.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParserAppelFonction.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParserArgFonction.h"
#include "Compilateur/Parsing/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>
#include "Compilateur/AST/Registre/RegistreInstruction.h"
#include "Compilateur/Parsing/Instruction/ParserScope.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParsingDeclarationFonction.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParsingReturn.h"
#include "Compilateur/Parsing/Instruction/Fonction/ParserArgPassFonction.h"
#include <filesystem>


int main(int argc, char* argv[])
{
    try {

        shared_ptr<LLVMBackend> backend = std::make_shared<LLVMBackend>();
        FichierLecture fichierLecture("../src/PrysmaCodeTests/main.p");
        std::string document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

        std::shared_ptr<RegistreInstruction> registreInstruction = std::make_shared<RegistreInstruction>();
        std::shared_ptr<RegistreVariable> registreVariable = std::make_shared<RegistreVariable>();
        std::shared_ptr<RegistreFonction> registreFonction = std::make_shared<RegistreFonction>();
        std::shared_ptr<RegistreType> registreType = std::make_shared<RegistreType>();
        std::shared_ptr<ReturnContextCompilation> returnContextCompilation = std::make_shared<ReturnContextCompilation>();
        std::shared_ptr<RegistreArgument> registreArgument = std::make_shared<RegistreArgument>();

        // Enregistrer des fonctions externe

        // PrintInt
        IntegerType* intTy = llvm::Type::getInt32Ty(backend->getContext());
        std::vector<llvm::Type*> printIntArgs = {intTy};
        backend->declarerExterne("printInt", intTy, printIntArgs);
        registreFonction->ajouter("printInt", backend->getModule().getFunction("printInt"));

        //PrintFloat 
        IntegerType* floatTy = llvm::Type::getInt32Ty(backend->getContext());
        std::vector<llvm::Type*> printFloatArgs = {floatTy};
        backend->declarerExterne("printFloat", floatTy, printFloatArgs);
        registreFonction->ajouter("printFloat", backend->getModule().getFunction("printFloat"));
        
        // Enregistrer les types de base
        registreType->enregistrer(TOKEN_TYPE_INT, llvm::Type::getInt32Ty(backend->getContext()));
        registreType->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(backend->getContext()));
        registreType->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(backend->getContext()));
        registreType->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(backend->getContext()));
        
        // Instruction du langage prysma
        registreInstruction->enregistrer(TOKEN_SCOPE, std::make_shared<ParserScope>());
        registreInstruction->enregistrer(TOKEN_FONCTION, std::make_shared<ParsingDeclarationFonction>(backend, registreFonction, registreVariable, registreType, TOKEN_FONCTION, returnContextCompilation));
        registreInstruction->enregistrer(TOKEN_AFF, std::make_shared<ParseurAffectationVariable>(backend, registreVariable,registreType));
        registreInstruction->enregistrer(TOKEN_DEC,std::make_shared<ParseurDeclarationVariable>(backend, registreVariable,registreType));
        registreInstruction->enregistrer(TOKEN_CALL, std::make_shared<ParserAppelFonction>(registreFonction, backend, registreArgument));
        registreInstruction->enregistrer(TOKEN_RETOUR, std::make_shared<ParsingReturn>(backend, registreVariable, returnContextCompilation, registreType));
        registreInstruction->enregistrer(TOKEN_ARG,std::make_shared<ParserArgFonction>(registreType));
        registreInstruction->enregistrer(TOKEN_PASS,std::make_shared<ParserArgPassFonction>(registreVariable, registreArgument));

        ConstructeurArbreInstruction constructeurArbreInstruction(registreInstruction);
        std::shared_ptr<INoeud> arbre = constructeurArbreInstruction.construire(tokens);

        arbre->genCode();

        LLVMSerializer serializer(backend->getContext(), backend->getModule());
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