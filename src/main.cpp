#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreStrategieEquation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieUnRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieNegation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieLitteral.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Boucle/ParseurWhile.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/LLVM/LlvmSerializer.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include "Compilateur/GestionnaireErreur.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>
#include <filesystem>
#include "Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include <cstdlib>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Erreur: Aucun fichier spécifié" << std::endl;
        return 1;
    }
    
    std::string cheminFichier = argv[1];
    std::string nomFichier = std::filesystem::path(cheminFichier).string(); 
    
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
      
        FichierLecture fichierLecture(cheminFichier);
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
        llvm::Type* floatTy = llvm::Type::getFloatTy(context->backend->getContext());
        std::vector<llvm::Type*> printFloatArgs = {floatTy};
        context->backend->declarerExterne("printFloat", floatTy, printFloatArgs);
        context->registreFonction->enregistrer("printFloat", context->backend->getModule().getFunction("printFloat"));

        // PrintBool
        llvm::Type* boolTy = llvm::Type::getInt1Ty(context->backend->getContext());
        std::vector<llvm::Type*> printBoolArgs = {boolTy};
        context->backend->declarerExterne("printBool", boolTy, printBoolArgs);
        context->registreFonction->enregistrer("printBool", context->backend->getModule().getFunction("printBool"));
        
        // Enregistrer les types de base
        context->registreType->enregistrer(TOKEN_TYPE_INT, llvm::Type::getInt32Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(context->backend->getContext()));
        
        // Créer le ParseurType avec le registre
        std::shared_ptr<ParseurType> parseurType = std::make_shared<ParseurType>(context->registreType);

        // Constuire les chef d'orchestre de l'arbre syntaxique abstrait
        ConstructeurArbreInstruction constructeurArbreInstruction(context->registreInstruction);
        std::shared_ptr<ConstructeurEquationFlottante> constructeurEquation = std::make_shared<ConstructeurEquationFlottante>(&constructeurArbreInstruction);
        
        // Enregistrer les strategies d'équation 
        std::shared_ptr<RegistreStrategieEquation> registreStrategieEquation = std::make_shared<RegistreStrategieEquation>();
        registreStrategieEquation->enregistrer(TOKEN_LIT_INT, std::make_shared<StrategieLitteral>());
        registreStrategieEquation->enregistrer(TOKEN_LIT_FLOAT, std::make_shared<StrategieLitteral>());
        registreStrategieEquation->enregistrer(TOKEN_LIT_BOLEEN, std::make_shared<StrategieLitteral>());
        registreStrategieEquation->enregistrer(TOKEN_IDENTIFIANT, std::make_shared<StrategieIdentifiant>(constructeurEquation));
        registreStrategieEquation->enregistrer(TOKEN_REF, std::make_shared<StrategieRef>());
        registreStrategieEquation->enregistrer(TOKEN_UNREF, std::make_shared<StrategieUnRef>());
        registreStrategieEquation->enregistrer(TOKEN_NON, std::make_shared<StrategieNegation>());
        registreStrategieEquation->enregistrer(TOKEN_CROCHET_OUVERT, std::make_shared<StrategieTableauInitialisation>(constructeurEquation->recupererConstructeurArbre()));
        ConstructeurEquationFlottante::setRegistreStrategieEquation(registreStrategieEquation);
    
        // NoeudInstruction du langage prysma
        context->registreInstruction->enregistrer(TOKEN_SCOPE, std::make_shared<ParseurScope>(&constructeurArbreInstruction));
        context->registreInstruction->enregistrer(TOKEN_FONCTION, std::make_shared<ParseurDeclarationFonction>(&constructeurArbreInstruction));
        context->registreInstruction->enregistrer(TOKEN_AFF, std::make_shared<ParseurAffectationVariable>(context->backend, context->registreVariable, context->registreType, constructeurEquation->recupererConstructeurArbre()));
        context->registreInstruction->enregistrer(TOKEN_DEC, std::make_shared<ParseurDeclarationVariable>(parseurType, constructeurEquation->recupererConstructeurArbre()));
        context->registreInstruction->enregistrer(TOKEN_CALL, std::make_shared<ParseurInstructionAppel>(constructeurEquation->recupererConstructeurArbre()));
        context->registreInstruction->enregistrer(TOKEN_RETOUR, std::make_shared<ParseurRetour>(constructeurEquation->recupererConstructeurArbre()));
        context->registreInstruction->enregistrer(TOKEN_ARG, std::make_shared<ParseurArgFonction>());
        context->registreInstruction->enregistrer(TOKEN_UNREF, std::make_shared<ParseurUnRefVariable>());
        context->registreInstruction->enregistrer(TOKEN_REF, std::make_shared<ParseurRefVariable>());
        context->registreInstruction->enregistrer(TOKEN_SI, std::make_shared<ParseurIf>(constructeurEquation->recupererConstructeurArbre(), &constructeurArbreInstruction));
        context->registreInstruction->enregistrer(TOKEN_TANT_QUE, std::make_shared<ParseurWhile>(constructeurEquation->recupererConstructeurArbre(), &constructeurArbreInstruction));

        // Ajouter la stratégie TOKEN_CALL 
        registreStrategieEquation->enregistrer(TOKEN_CALL, std::make_shared<StrategieAppelFonction>(constructeurEquation->recupererConstructeurArbre()));

        // Construire l'arbre syntaxique 
        std::shared_ptr<INoeud> arbre = constructeurArbreInstruction.construire(tokens);

        VisiteurGeneralGenCode visiteur(context);
        arbre->accept(&visiteur);

        SortieGrapheVisuelTexte sortieGrapheVisuel("output.dot");
        VisiteurGeneralGraphViz visiteurGraphViz(std::move(sortieGrapheVisuel));
        arbre->accept(&visiteurGraphViz);
        visiteurGraphViz.generer();

        (void)system("dot -Tpng output.dot -o ast_graph.png");

        LlvmSerializer serializer(context->backend->getContext(), context->backend->getModule());
        serializer.SauvegarderCodeLLVM("output.ll");
       
        ConstructeurSysteme constructeur("../src/Lib", "Lib", "output.ll", "programme");
        constructeur.compilerLib();
        constructeur.lierLibExecutable();
    }
    catch (const ErreurCompilation& erreur) {
        std::cerr << nomFichier << ":" 
                  << erreur.ligne << ":" 
                  << erreur.colonne << ": Erreur: " 
                  << erreur.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}