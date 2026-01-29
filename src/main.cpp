#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
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
        FichierLecture fichierLecture("src/PrysmaCodeTests/main.prysma");
        document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

        // ===== Construction de l'arbre d'expression =====
        std::unique_ptr<FloatEquationBuilder> floatEquationBuilder = std::make_unique<FloatEquationBuilder>(backend->getContext());
        shared_ptr<INoeud> expression = floatEquationBuilder->builderArbreEquationFloat(tokens);
        
        // ===== Création de la fonction main LLVM =====
        backend->creationFonctionMain();

        // ===== Résolution de l'expression =====

        NoeudDeclaration noeudDeclaration(backend,"teste",backend->getBuilder().getFloatTy(), expression->genCode()); 
        llvm::Value* testePtr =  noeudDeclaration.genCode();
        
        // Charger la valeur depuis le pointeur
        llvm::Value* teste = backend->getBuilder().CreateLoad(backend->getBuilder().getFloatTy(), testePtr, "teste_value");
        
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