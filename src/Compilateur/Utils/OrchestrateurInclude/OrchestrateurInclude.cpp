#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreVariable.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
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
#include <filesystem>
#include <iostream>

OrchestrateurInclude::OrchestrateurInclude(FacadeConfigurationEnvironnement* facadeConfigurationEnvironnement, RegistreFonctionGlobale* registreFonctionGlobale, RegistreFichier* registreFichier) : 
_facadeConfigurationEnvironnement(facadeConfigurationEnvironnement),
_registreFonctionGlobale(registreFonctionGlobale),
_registreFichier(registreFichier)
{}

OrchestrateurInclude::~OrchestrateurInclude()
{}

void OrchestrateurInclude::nouvelleInstance(const std::string& cheminFichier) {

    // Résoudre le chemin absolu du fichier
    std::filesystem::path cheminAbsolu = std::filesystem::absolute(cheminFichier);
    
    // Si le chemin est relatif et qu'on a un répertoire courant, résoudre par rapport à celui-ci
    if (!std::filesystem::exists(cheminAbsolu) && !_repertoireCourant.empty()) {
        cheminAbsolu = std::filesystem::path(_repertoireCourant) / cheminFichier;
    }
    
    std::string cheminResolu = cheminAbsolu.string();
    
    // Sauvegarder le répertoire courant pour les includes imbriqués
    std::string ancienRepertoire = _repertoireCourant;
    _repertoireCourant = cheminAbsolu.parent_path().string();

    std::string pathProgramme = "programme/";
    std::string pathGraphe = "graphe/";
    
    std::string nomFichier = cheminResolu.substr(cheminResolu.find_last_of("/\\") + 1);
    nomFichier = nomFichier.substr(0, nomFichier.find_last_of('.'));

    _registreFichier->ajouterFichier(pathProgramme + nomFichier + ".ll");

    _facadeConfigurationEnvironnement->initialiser();

    ContextGenCode* context = _facadeConfigurationEnvironnement->getContext();
    ConstructeurArbreInstruction* constructeurArbreInstruction = _facadeConfigurationEnvironnement->getConstructeurArbreInstruction();

    FichierLecture fichierLecture(cheminResolu);
    std::string document = fichierLecture.entrer();

    Lexer lexer;
    vector<Token> tokens = lexer.tokenizer(document);

    INoeud* arbre = constructeurArbreInstruction->construire(tokens);

    VisiteurRemplissageRegistre visiteurRemplissageRegistre(context, this);
    arbre->accept(&visiteurRemplissageRegistre);

    ConstructeurEnvironnementRegistreFonction constructeurEnvironnementRegistreFonction(context);
    constructeurEnvironnementRegistreFonction.remplir();

    ConstructeurEnvironnementRegistreVariable constructeurEnvironnementRegistreVariable(context);
    constructeurEnvironnementRegistreVariable.remplir();

    VisiteurGeneralGenCode visiteur(context, this);
    arbre->accept(&visiteur);

    SortieGrapheVisuelTexte sortieGrapheVisuel(pathGraphe+nomFichier+".dot");
    VisiteurGeneralGraphViz visiteurGraphViz(std::move(sortieGrapheVisuel));
    arbre->accept(&visiteurGraphViz);
    visiteurGraphViz.generer();

    if (system(("dot -Tsvg " + pathGraphe + nomFichier + ".dot -o " + pathGraphe + nomFichier + ".svg").c_str()) != 0) {
        std::cerr << "Erreur lors de la génération du graphe." << std::endl;
    }

    // Supprimer le fichier .dot intermédiaire
    if (system(("rm " + pathGraphe + nomFichier + ".dot").c_str()) != 0) {
        std::cerr << "Erreur lors de la suppression du fichier .dot." << std::endl;
    }

    LlvmSerializer serializer(context->backend->getContext(), context->backend->getModule());
    serializer.SauvegarderCodeLLVM(pathProgramme + nomFichier + ".ll"); 

    // Restaurer le répertoire courant pour les includes au même niveau
    _repertoireCourant = ancienRepertoire;
}

void OrchestrateurInclude::inclureFichier(const std::string& cheminAbsolu) {
    
    // Sécurité pour éviter de compiler en boucle infinie, évite d'inclure plusieurs fois le même fichier
    if (_registreFichier->verifierFichier(cheminAbsolu)) {
        return; 
    }

    // Créer un environnement indépendant pour ne pas corrompre celui du parent
    auto facadeInclude = std::make_unique<FacadeConfigurationEnvironnement>(_registreFonctionGlobale, _registreFichier);

    // Substituer temporairement la facade
    FacadeConfigurationEnvironnement* ancienneFacade = _facadeConfigurationEnvironnement;
    _facadeConfigurationEnvironnement = facadeInclude.get();

    // Réutiliser nouvelleInstance avec la facade temporaire
    nouvelleInstance(cheminAbsolu);

    // Restaurer la facade du parent
    _facadeConfigurationEnvironnement = ancienneFacade;

    // Sauvegarder la mémoire de l'environnement enfant, vitale pour la gestion de la mémoire, delete automatiquement à la fin de vie de l'instance
    // sert de support de vie pour les objets alloués dans l'environnement enfant
    // évite la destruction de la mémoire à la fin du scope de la méthode, ce qui corromprait la mémoire du parent(segmentation fault)
    _facadesEnfants.push_back(std::move(facadeInclude));
}