#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"

#include <iostream>
#include <memory>

using namespace std;


void parse()
{
    
}
int main() {
    try {
        LLVMBackend backend;

        std::string document;
        FichierLecture fichierLecture("../src/PrysmaCodeTests/main.prysma");
        document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

        // ===== Construction de l'arbre d'expression =====
        std::unique_ptr<FloatEquationBuilder> floatEquationBuilder = std::make_unique<FloatEquationBuilder>(backend.getContext());
        shared_ptr<INoeud> expression = floatEquationBuilder->builderArbreEquationFloat(tokens);
        
        // ===== Création de la fonction main LLVM =====
        backend.creationFonctionMain();

        // ===== Résolution de l'expression =====
        llvm::Value* resultatNumerique = expression->genCode();
        
        // ===== Affichage du résultat avec printf =====
        backend.print(resultatNumerique);

        // ===== Retour du résultat =====
        llvm::Value* resultInt = backend.getBuilder().CreateFPToSI(resultatNumerique, backend.getBuilder().getInt32Ty(), "resultInt");
        backend.getBuilder().CreateRet(resultInt);

        backend.sauvegarderCodeLLVM("output.ll");

        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}