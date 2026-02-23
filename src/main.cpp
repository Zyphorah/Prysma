#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieString.h"
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
#include <filesystem>
#include "Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "llvm/Support/Allocator.h"
#include <cstdlib>

// Améliorations possibles performance : 
// DFA pour le lexer permet de gagner 10 à 20 fois supérieur en vitesse de tokenisation par rapport à l'approche de base 
// Pour un gain de 20 pourcents en vitesse 
// Faire un système de pool string pour le système complet au lieu d'utiliser les strings lente de std::string qui ne sont pas contigue en mémoire et qui peuvent causer des problèmes de performance et de fragmentation mémoire.
// Utilisation du polymorphisme statique avec des templates pour la vitesse d'exécution
// Utilisation d'un système de table compile time au lieu des registres dynamiques c'est plus rapide 10 20 pourcent de gain en vitesse d'exécution
// Swiss Tables pour les tables de hachage compile time
// Utilisation d'un DFA mais avec SIMD (Single Instruction, Multiple Data), pour un maximum de gain de performance
// Utilisation d'un système multi thread pour la compilation, un thread par fichier source. 
// Transformer la structure de l'arbre syntaxique abstrait en "SoA" (Structure of Arrays) amélioration de la localité du cache et de la performance d'exécution. 

int main(int argc, char* argv[])
{

    // Faire un système d'héritage pour les visiteur, une classe de base qui contient toute les définition de visiteur 
    // ça va éviter d'avoir une dupplication de code, ou une définition de contrat pour les visiteur que je veux seulement faire une action précise, 
    // exemple remplir un registre de fonction, ou remplir un registre de variable, tout les registres peuvent être rempli en avance puis ensuite 
    // utiliser une technique de multi threade pour faire la génération de code en parallèle, le but est d'avoir un système découplé pour ne pas 
    // avoir de problème de race condition. 

    if (argc < 2) {
        std::cerr << "Erreur: Aucun fichier spécifié" << std::endl;
        return 1;
    }
    
    // Arena bump allocator pour optimiser les performances d'allocation mémoire
    llvm::BumpPtrAllocator arena;
    
    std::string cheminFichier = argv[1];
    std::string nomFichier = std::filesystem::path(cheminFichier).string(); 
    
    try {
        auto* backend = new (arena.Allocate<LlvmBackend>()) LlvmBackend();
        auto* registreInstruction = new (arena.Allocate<RegistreInstruction>()) RegistreInstruction();
        auto* registreVariable = new (arena.Allocate<RegistreVariable>()) RegistreVariable();
        auto* registreFonction = new (arena.Allocate<RegistreFonction>()) RegistreFonction();
        auto* registreType = new (arena.Allocate<RegistreType>()) RegistreType();
        auto* returnContextCompilation = new (arena.Allocate<RetourContexteCompilation>()) RetourContexteCompilation();
        auto* registreArgument = new (arena.Allocate<RegistreArgument>()) RegistreArgument();
        Symbole valeurTemporaire;
        valeurTemporaire.adresse = nullptr;
        valeurTemporaire.type = nullptr;

        auto* context = new (arena.Allocate<ContextGenCode>()) ContextGenCode(
            backend,
            registreInstruction,
            registreVariable,
            registreFonction,
            registreType,
            returnContextCompilation,
            registreArgument,
            valeurTemporaire,
            &arena
        );
      
        FichierLecture fichierLecture(cheminFichier);
        std::string document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

        // Enregistrer des fonctions externes

        // backSlashN
        context->backend->declarerExterne("backSlashN", llvm::Type::getVoidTy(context->backend->getContext()), {});
        {
            SymboleFonction symBackSlashN{};
            symBackSlashN.fonction = context->backend->getModule().getFunction("backSlashN");
            symBackSlashN.typeRetour = nullptr;  
            context->registreFonction->enregistrer("backSlashN", symBackSlashN);
        }

        // print
        std::vector<llvm::Type*> print_args;
        print_args.push_back(llvm::Type::getInt8Ty(context->backend->getContext()));
        llvm::FunctionType* print_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context->backend->getContext()), print_args, true);
        llvm::Function* printFunc = llvm::Function::Create(print_type, llvm::Function::ExternalLinkage, "print", context->backend->getModule());
        {
            SymboleFonction symPrint{};
            symPrint.fonction = printFunc;
            symPrint.typeRetour = nullptr;  
            context->registreFonction->enregistrer("print", symPrint);
        }

        // Enregistrer les types de base
        context->registreType->enregistrer(TOKEN_TYPE_STRING,llvm::Type::getInt8Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_CHAR, llvm::Type::getInt8Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_INT64, llvm::Type::getInt64Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_INT32, llvm::Type::getInt32Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_FLOAT, llvm::Type::getFloatTy(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_BOOL, llvm::Type::getInt1Ty(context->backend->getContext()));
        context->registreType->enregistrer(TOKEN_TYPE_VOID, llvm::Type::getVoidTy(context->backend->getContext()));
       
        // Constuire les chef d'orchestre de l'arbre syntaxique abstrait
        auto* rawConstructeurArbreInstruction = new (arena) 
            ConstructeurArbreInstruction(context->registreInstruction, arena);
        
        auto* constructeurEquation = new (arena) 
            ConstructeurEquationFlottante(rawConstructeurArbreInstruction, arena);
        
        // Créer le ParseurType avec le registre
        auto* parseurType = new (arena.Allocate<ParseurType>()) 
            ParseurType(context->registreType, constructeurEquation->recupererConstructeurArbre());
        // Enregistrer les strategies d'équation 
        auto* registreStrategieEquation = new (arena.Allocate<RegistreStrategieEquation>()) RegistreStrategieEquation();
        
        auto* stratLitInt = new (arena.Allocate<StrategieLitteral>()) StrategieLitteral(arena);
        registreStrategieEquation->enregistrer(TOKEN_LIT_INT, stratLitInt);
        
        auto* stratLitFloat = new (arena.Allocate<StrategieLitteral>()) StrategieLitteral(arena);
        registreStrategieEquation->enregistrer(TOKEN_LIT_FLOAT, stratLitFloat);
        
        auto* stratLitBool = new (arena.Allocate<StrategieLitteral>()) StrategieLitteral(arena);
        registreStrategieEquation->enregistrer(TOKEN_LIT_BOLEEN, stratLitBool);
        
        auto* stratIdent = new (arena.Allocate<StrategieIdentifiant>()) StrategieIdentifiant(constructeurEquation);
        registreStrategieEquation->enregistrer(TOKEN_IDENTIFIANT, stratIdent);
        
        auto* stratRef = new (arena.Allocate<StrategieRef>()) StrategieRef(arena);
        registreStrategieEquation->enregistrer(TOKEN_REF, stratRef);
        
        auto* stratUnRef = new (arena.Allocate<StrategieUnRef>()) StrategieUnRef(arena);
        registreStrategieEquation->enregistrer(TOKEN_UNREF, stratUnRef);
        
        auto* stratNeg = new (arena.Allocate<StrategieNegation>()) StrategieNegation(constructeurEquation);
        registreStrategieEquation->enregistrer(TOKEN_NON, stratNeg);
        
        auto* stratTab = new (arena.Allocate<StrategieTableauInitialisation>()) StrategieTableauInitialisation(constructeurEquation->recupererConstructeurArbre());
        registreStrategieEquation->enregistrer(TOKEN_CROCHET_OUVERT, stratTab);

        auto* stratString = new (arena.Allocate<StrategieString>()) StrategieString(arena);
        registreStrategieEquation->enregistrer(TOKEN_GUILLEMET, stratString);
        
        ConstructeurEquationFlottante::setRegistreStrategieEquation(registreStrategieEquation);
    
        // NoeudInstruction du langage prysma
        auto* parsScope = new (arena.Allocate<ParseurScope>()) ParseurScope(rawConstructeurArbreInstruction);
        context->registreInstruction->enregistrer(TOKEN_SCOPE, parsScope);
        
        auto* parsFonc = new (arena.Allocate<ParseurDeclarationFonction>()) ParseurDeclarationFonction(rawConstructeurArbreInstruction, parseurType);
        context->registreInstruction->enregistrer(TOKEN_FONCTION, parsFonc);
        
        auto* parsAff = new (arena.Allocate<ParseurAffectationVariable>()) ParseurAffectationVariable(context->backend, context->registreVariable, context->registreType, constructeurEquation->recupererConstructeurArbre());
        context->registreInstruction->enregistrer(TOKEN_AFF, parsAff);
        
        auto* parsDec = new (arena.Allocate<ParseurDeclarationVariable>()) ParseurDeclarationVariable(parseurType, constructeurEquation->recupererConstructeurArbre());
        context->registreInstruction->enregistrer(TOKEN_DEC, parsDec);
        
        auto* parsCall = new (arena.Allocate<ParseurInstructionAppel>()) ParseurInstructionAppel(constructeurEquation->recupererConstructeurArbre());
        context->registreInstruction->enregistrer(TOKEN_CALL, parsCall);
        
        auto* parsRet = new (arena.Allocate<ParseurRetour>()) ParseurRetour(constructeurEquation->recupererConstructeurArbre());
        context->registreInstruction->enregistrer(TOKEN_RETOUR, parsRet);
        
        auto* parsArg = new (arena.Allocate<ParseurArgFonction>()) ParseurArgFonction(parseurType, arena);
        context->registreInstruction->enregistrer(TOKEN_ARG, parsArg);
        
        auto* parsUnRef = new (arena.Allocate<ParseurUnRefVariable>()) ParseurUnRefVariable(arena);
        context->registreInstruction->enregistrer(TOKEN_UNREF, parsUnRef);
        
        auto* parsRefVar = new (arena.Allocate<ParseurRefVariable>()) ParseurRefVariable(arena);
        context->registreInstruction->enregistrer(TOKEN_REF, parsRefVar);
        
        auto* parsIf = new (arena.Allocate<ParseurIf>()) ParseurIf(constructeurEquation->recupererConstructeurArbre(), rawConstructeurArbreInstruction);
        context->registreInstruction->enregistrer(TOKEN_SI, parsIf);
        
        auto* parsWhile = new (arena.Allocate<ParseurWhile>()) ParseurWhile(constructeurEquation->recupererConstructeurArbre(), rawConstructeurArbreInstruction);
        context->registreInstruction->enregistrer(TOKEN_TANT_QUE, parsWhile);

        // Ajouter la stratégie TOKEN_CALL 
        auto* stratCall = new (arena.Allocate<StrategieAppelFonction>()) StrategieAppelFonction(constructeurEquation->recupererConstructeurArbre());
        registreStrategieEquation->enregistrer(TOKEN_CALL, stratCall);

        // Construire l'arbre syntaxique 
        INoeud* arbre = rawConstructeurArbreInstruction->construire(tokens);

        VisiteurGeneralGenCode visiteur(context);
        arbre->accept(&visiteur);

        SortieGrapheVisuelTexte sortieGrapheVisuel("output.dot");
        VisiteurGeneralGraphViz visiteurGraphViz(std::move(sortieGrapheVisuel));
        arbre->accept(&visiteurGraphViz);
        visiteurGraphViz.generer();

        if (system("dot -Tpng output.dot -o ast_graph.png") != 0) {
            std::cerr << "Erreur lors de la génération du graphe AST." << std::endl;
        }

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