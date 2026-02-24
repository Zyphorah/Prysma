#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieString.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/ConstructeurArbreInstruction.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include "Compilateur/AST/Registre/RegistreArgument.h"
#include "Compilateur/AST/Registre/RegistreStrategieEquation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieUnRef.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieNegation.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieLitteral.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurRetour.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Boucle/ParseurWhile.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurIf.h"
#include "Compilateur/GrapheVisuel/SortieGrapheVisuelTexte.h"
#include "Compilateur/LLVM/LlvmBackend.h"
#include "Compilateur/LLVM/LlvmSerializer.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieTableauInitialisation.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <cstdlib>
#include <string>
#include <memory>

OrchestrateurInclude::~OrchestrateurInclude()
{
    
}

void OrchestrateurInclude::nouvelleInstance(const std::string& cheminFichier) {
    // Initialisation des différents composants du compilateur
      // Arena bump allocator pour optimiser les performances d'allocation mémoire
        llvm::BumpPtrAllocator arena;
    
        auto backend = std::make_unique<LlvmBackend>();
        auto registreInstruction = std::make_unique<RegistreInstruction>();
        // note la différence entre le registre de fonction globale et locale, c'est que le globale ce sont tout les fonctions de tout les fichier sans 
        // de llvm::Fonction qui cause problème pour le multi thread, alors que le local c'est les fonctions d'un fichier local courent du thread
        auto registreVariable = std::make_unique<RegistreVariable>();
      
    
        
        auto registreFonctionLocale = std::make_unique<RegistreFonction>();
        auto registreType = std::make_unique<RegistreType>();
        auto returnContextCompilation = std::make_unique<RetourContexteCompilation>();
        auto registreArgument = std::make_unique<RegistreArgument>();
        Symbole valeurTemporaire;
        valeurTemporaire.adresse = nullptr;
        valeurTemporaire.type = nullptr;

        auto context = std::make_unique<ContextGenCode>(
            backend.get(),
            registreInstruction.get(),
            registreVariable.get(),
            registreFonctionGlobale,
            registreFonctionLocale.get(),
            registreType.get(),
            returnContextCompilation.get(),
            registreArgument.get(),
            valeurTemporaire,
            &arena
        );
      
        FichierLecture fichierLecture(cheminFichier);
        std::string document = fichierLecture.entrer();
  
        Lexer lexer;
        vector<Token> tokens = lexer.tokenizer(document);

        // Enregistrer des fonctions externes

        // Créer un vrai type Void pour Prysma
        IType* typeVoidPrysma = new (arena.Allocate<TypeSimple>()) TypeSimple(llvm::Type::getVoidTy(context->backend->getContext()));

        // backSlashN
        context->backend->declarerExterne("backSlashN", llvm::Type::getVoidTy(context->backend->getContext()), {});
        {
            // Registre global (thread safe, pas de llvm::Function*)
            auto symBackSlashNGlobal = std::make_unique<SymboleFonctionGlobale>();
            symBackSlashNGlobal->typeRetour = typeVoidPrysma;
            symBackSlashNGlobal->noeud = nullptr;
            context->registreFonctionGlobale->enregistrer("backSlashN", std::move(symBackSlashNGlobal));

            // Registre local (avec llvm::Function* pour la génération de code)
            auto symBackSlashNLocal = std::make_unique<SymboleFonctionLocale>();
            symBackSlashNLocal->fonction = context->backend->getModule().getFunction("backSlashN");
            symBackSlashNLocal->typeRetour = typeVoidPrysma;
            symBackSlashNLocal->noeud = nullptr;
            context->registreFonctionLocale->enregistrer("backSlashN", std::move(symBackSlashNLocal));
        }

        // print
        std::vector<llvm::Type*> print_args;
        print_args.push_back(llvm::Type::getInt8Ty(context->backend->getContext()));
        llvm::FunctionType* print_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context->backend->getContext()), print_args, true);
        llvm::Function* printFunc = llvm::Function::Create(print_type, llvm::Function::ExternalLinkage, "print", context->backend->getModule());
        {
            // Registre global (thread safe, pas de llvm::Function*)
            auto symPrintGlobal = std::make_unique<SymboleFonctionGlobale>();
            symPrintGlobal->typeRetour = typeVoidPrysma;
            symPrintGlobal->noeud = nullptr;
            context->registreFonctionGlobale->enregistrer("print", std::move(symPrintGlobal));

            // Registre local (avec llvm::Function* pour la génération de code)
            auto symPrintLocal = std::make_unique<SymboleFonctionLocale>();
            symPrintLocal->fonction = printFunc;
            symPrintLocal->typeRetour = typeVoidPrysma;
            symPrintLocal->noeud = nullptr;
            context->registreFonctionLocale->enregistrer("print", std::move(symPrintLocal));
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

        VisiteurRemplissageRegistre visiteurRemplissageRegistre(context.get());
        arbre->accept(&visiteurRemplissageRegistre);

        ConstructeurEnvironnementRegistreFonction constructeurEnvironnementRegistreFonction(context.get());
        constructeurEnvironnementRegistreFonction.remplir();

        ConstructeurEnvironnementRegistreVariable constructeurEnvironnementRegistreVariable(context.get());
        constructeurEnvironnementRegistreVariable.remplir();
        
        VisiteurGeneralGenCode visiteur(context.get());
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