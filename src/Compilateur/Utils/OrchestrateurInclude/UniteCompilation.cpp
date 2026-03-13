#include "Compilateur/AST/Utils/OrchestrateurInclude/UniteCompilation.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"
#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreVariable.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/GrapheVisuel/SortieGrapheVisuelTexte.h"
#include "Compilateur/LLVM/LlvmSerializer.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Visiteur/VisiteurRemplissageCorpsClass/VisiteurRemplissageCorpsClass.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>

UniteCompilation::UniteCompilation(OrchestrateurInclude* orchestrateur, RegistreFichier* registre, std::string cheminFichier, RegistreFonctionGlobale* registreFonctionGlobale) 
    : _orchestrateur(orchestrateur), 
      _registreFichier(registre),
      _context(nullptr),
      _arbre(nullptr),
      _cheminFichierOriginal(std::move(cheminFichier)) 
{
    // Initialisation du contexte séparé (la bulle privée)
    _facadeConfigurationEnvironnement = std::make_unique<FacadeConfigurationEnvironnement>(registreFonctionGlobale, _registreFichier);
}

UniteCompilation::~UniteCompilation() 
{
    _arbre = nullptr; 
    _context = nullptr; 
}

void UniteCompilation::passe1() {
    std::filesystem::path cheminAbsolu = std::filesystem::absolute(_cheminFichierOriginal);
    
    if (!std::filesystem::exists(cheminAbsolu) && !_repertoireCourant.empty()) {
        cheminAbsolu = std::filesystem::path(_repertoireCourant) / _cheminFichierOriginal;
    }
    
    std::string cheminResolu = cheminAbsolu.string();
    
    _ancienRepertoire = _repertoireCourant;
    _repertoireCourant = cheminAbsolu.parent_path().string();

    // Construire un nom de fichier unique en incluant le nom du dossier parent 
    // pour éviter les collisions quand plusieurs fichiers ont le même nom dans des dossiers différents
    // Ex: ParentA/EnfantA.p -> "ParentA_EnfantA", ParentB/EnfantA.p -> "ParentB_EnfantA"
    std::string nomBase = cheminAbsolu.stem().string();
    std::string nomParent = cheminAbsolu.parent_path().filename().string();
    _nomFichier = nomParent + "_" + nomBase;

    _registreFichier->ajouterFichier("programme/" + _nomFichier + ".ll");

    _facadeConfigurationEnvironnement->initialiser(cheminResolu);

    _context = _facadeConfigurationEnvironnement->getContext();
    ConstructeurArbreInstruction* constructeurArbreInstruction = _facadeConfigurationEnvironnement->getConstructeurArbreInstruction();

    FichierLecture fichierLecture(cheminResolu);
    std::string document = fichierLecture.entrer();

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenizer(document);

    _arbre = constructeurArbreInstruction->construire(tokens);

    VisiteurRemplissageRegistre visiteurRemplissageRegistre(_context, _orchestrateur);
    _arbre->accept(&visiteurRemplissageRegistre);
    VisiteurRemplissageCoprsClass visiteurRemplissageCorpsClass(_context);
    _arbre->accept(&visiteurRemplissageCorpsClass);
}

void UniteCompilation::passe2() {
    if (_arbre == nullptr) 
    {
        return;
    }

    ConstructeurEnvironnementRegistreFonction constructeurEnvironnementRegistreFonction(_context);
    constructeurEnvironnementRegistreFonction.remplir();

    ConstructeurEnvironnementRegistreVariable constructeurEnvironnementRegistreVariable(_context);
    constructeurEnvironnementRegistreVariable.remplir();

    std::filesystem::path buildDir = std::filesystem::canonical("/proc/self/exe").parent_path();
    std::string pathProgramme = (buildDir / "programme/").string();
    std::string pathGraphe = (buildDir / "graphe/").string();

    VisiteurGeneralGenCode visiteur(_context, _orchestrateur);
    _arbre->accept(&visiteur);

    if (_orchestrateur->estGraphVizActif()) {
        SortieGrapheVisuelTexte sortieGrapheVisuel(pathGraphe + _nomFichier + ".dot");
        auto visiteurGraphViz = std::make_unique<VisiteurGeneralGraphViz>(std::move(sortieGrapheVisuel));
        _arbre->accept(visiteurGraphViz.get());
        visiteurGraphViz->generer();

        if (system(("dot -Tsvg " + pathGraphe + _nomFichier + ".dot -o " + pathGraphe + _nomFichier + ".svg").c_str()) != 0) {
            std::cerr << "Erreur lors de la génération du graphe." << std::endl;
        }

        // Supprimer le fichier .dot intermédiaire
        std::filesystem::remove(pathGraphe + _nomFichier + ".dot");
    }

    LlvmSerializer serializer(_context->backend->getContext(), _context->backend->getModule());
    serializer.SauvegarderCodeLLVM(pathProgramme + _nomFichier + ".ll"); 

    // Restaurer le répertoire courant pour les includes au même niveau
    _repertoireCourant = _ancienRepertoire;
}

std::string UniteCompilation::getChemin() const
{
    return _cheminFichierOriginal;
}