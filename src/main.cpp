#include "Compilateur/AST/Registre/RegistreVariable.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/Parsing/Instruction/Variable/ParseurVariableFloat.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include <iostream>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>

using namespace std;

int main() {
    try {
        shared_ptr<LLVMBackend> backend = std::make_shared<LLVMBackend>();

        std::string document;
        FichierLecture fichierLecture("../src/PrysmaCodeTests/main.prysma");
        document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

     
        // ===== Création de la fonction main LLVM =====
        backend->creationFonctionMain();

        // ===== Résolution de l'expression =====

        int index = 0; 

        ParseurVariableFloat parseurVariableFloat(backend);

        shared_ptr<INoeud> noeud = parseurVariableFloat.parser(tokens, index, nullptr);

        llvm::Value* valeur = noeud->genCode();
        
        // Charger la valeur depuis le pointeur
        llvm::Value* teste = backend->getBuilder().CreateLoad(backend->getBuilder().getFloatTy(), valeur, "teste_value");
        
        // llvm::Value* resultatNumerique = expression->genCode();
        
        // ===== Affichage du résultat avec printf =====
        backend->print(teste);

        // ===== Retour du résultat =====
        llvm::Value* resultInt = backend->getBuilder().CreateFPToSI(teste, backend->getBuilder().getInt32Ty(), "resultInt");
        backend->getBuilder().CreateRet(resultInt);

        backend->sauvegarderCodeLLVM("output.ll");

        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}