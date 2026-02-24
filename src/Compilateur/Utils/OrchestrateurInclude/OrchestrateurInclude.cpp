#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreVariable.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/GrapheVisuel/SortieGrapheVisuelTexte.h"
#include "Compilateur/LLVM/LlvmSerializer.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <cstdlib>
#include <string>

OrchestrateurInclude::OrchestrateurInclude(FacadeConfigurationEnvironnement* facadeConfigurationEnvironnement, RegistreFonction* registreFonctionGlobale) : 
_facadeConfigurationEnvironnement(facadeConfigurationEnvironnement),
_registreFonctionGlobale(registreFonctionGlobale)
{}

OrchestrateurInclude::~OrchestrateurInclude()
{}

void OrchestrateurInclude::nouvelleInstance(const std::string& cheminFichier) {
    _facadeConfigurationEnvironnement->initialiser();

    ContextGenCode* context = _facadeConfigurationEnvironnement->getContext();
    ConstructeurArbreInstruction* constructeurArbreInstruction = _facadeConfigurationEnvironnement->getConstructeurArbreInstruction();

    FichierLecture fichierLecture(cheminFichier);
    std::string document = fichierLecture.entrer();

    Lexer lexer;
    vector<Token> tokens = lexer.tokenizer(document);

    INoeud* arbre = constructeurArbreInstruction->construire(tokens);

    VisiteurRemplissageRegistre visiteurRemplissageRegistre(context);
    arbre->accept(&visiteurRemplissageRegistre);

    ConstructeurEnvironnementRegistreFonction constructeurEnvironnementRegistreFonction(context);
    constructeurEnvironnementRegistreFonction.remplir();

    ConstructeurEnvironnementRegistreVariable constructeurEnvironnementRegistreVariable(context);
    constructeurEnvironnementRegistreVariable.remplir();

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
}
