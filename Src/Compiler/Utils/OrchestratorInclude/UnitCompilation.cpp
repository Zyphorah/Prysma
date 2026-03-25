#include "Compiler/AST/Utils/OrchestratorInclude/UnitCompilation.h"
#include "Compiler/AST/BuilderTreeInstruction.h"
#include "Compiler/AST/Utils/BuilderEnvironmentRegistryFunction.h"
#include "Compiler/AST/Utils/BuilderEnvironmentRegistryVariable.h"
#include "Compiler/AST/Utils/OrchestratorInclude/ConfigurationFacadeEnvironment.h"
#include "Compiler/AST/Utils/OrchestratorInclude/OrchestratorInclude.h"
#include "Compiler/FileProcessing/FileReading.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/VisualGraph/OutputVisualGraphText.h"
#include "Compiler/LLVM/LlvmSerializer.h"
#include "Compiler/Visitor/ASTGraphViz/GeneralVisitorGraphViz.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/Visitor/VisitorFillingBodyClass/VisitorFillingBodyClass.h"
#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <utility>
#include <vector>

UnitCompilation::UnitCompilation(OrchestratorInclude* orchestrator, RegistryFile* registry, std::string cheminFile, RegistryFunctionGlobale* registryFunctionGlobale) 
    : _orchestrator(orchestrator), 
      _registryFile(registry),
      _context(nullptr),
      _tree(nullptr),
      _cheminFileOriginal(std::move(cheminFile)) 
{
    // Initialization du contexte séparé (la bulle privée)
    _facadeConfigurationEnvironnement = std::make_unique<ConfigurationFacadeEnvironnement>(registryFunctionGlobale, _registryFile);
}

UnitCompilation::~UnitCompilation() 
{
    _tree = nullptr; 
    _context = nullptr; 
}

void UnitCompilation::passe1() {
    std::filesystem::path cheminAbsolu = std::filesystem::absolute(_cheminFileOriginal);
    
    if (!std::filesystem::exists(cheminAbsolu) && !_repertoireCourant.empty()) {
        cheminAbsolu = std::filesystem::path(_repertoireCourant) / _cheminFileOriginal;
    }
    
    std::string cheminResolu = cheminAbsolu.string();
    
    _ancienRepertoire = _repertoireCourant;
    _repertoireCourant = cheminAbsolu.parent_path().string();

    // Construire un nom de fichier unique en incluant le nom du dossier parent 
    // pour éviter les collisions quand plusieurs fichiers ont le même nom dans des dossiers différents
    // Ex: ParentA/ChildA.p -> "ParentA_ChildA", ParentB/ChildA.p -> "ParentB_ChildA"
    std::string nomBase = cheminAbsolu.stem().string();
    std::string nomParent = cheminAbsolu.parent_path().filename().string();
    _nomFile = nomParent + "_" + nomBase;

    _registryFile->ajouterFile("programme/" + _nomFile + ".ll");

    _facadeConfigurationEnvironnement->initialiser(cheminResolu);

    _context = _facadeConfigurationEnvironnement->getContext();
    BuilderTreeInstruction* builderTreeInstruction = _facadeConfigurationEnvironnement->getBuilderTreeInstruction();

    FileReading fichierReading(cheminResolu);
    std::string document = fichierReading.inputr();

    std::vector<Token> tokens = Lexer::tokenizer(document);

    _tree = builderTreeInstruction->construire(tokens);

    FillingVisitorRegistry visitorFillingRegistry(_context, _orchestrator);
    _tree->accept(&visitorFillingRegistry);
    FillingVisitorCoprsClass visitorFillingBodyClass(_context);
    _tree->accept(&visitorFillingBodyClass);
}

void UnitCompilation::passe2() {
    if (_tree == nullptr) 
    {
        return;
    }

    BuilderEnvironmentRegistryFunction builderEnvironnementRegistryFunction(_context);
    builderEnvironnementRegistryFunction.remplir();

    BuilderEnvironmentRegistryVariable builderEnvironnementRegistryVariable(_context);
    builderEnvironnementRegistryVariable.remplir();

    std::filesystem::path buildDir = std::filesystem::canonical("/proc/self/exe").parent_path();
    std::string pathProgramme = (buildDir / "programme/").string();
    std::string pathGraph = (buildDir / "graphe/").string();

    GeneralVisitorGenCode visitor(_context, _orchestrator);
    _tree->accept(&visitor);

    if (_orchestrator->estGraphVizActif()) {
        OutputVisualGraphText outputVisualGraph(pathGraph + _nomFile + ".dot");
        auto visitorGraphViz = std::make_unique<GeneralVisitorGraphViz>(std::move(outputVisualGraph));
        _tree->accept(visitorGraphViz.get());
        visitorGraphViz->generatedr();

        if (system(("dot -Tsvg " + pathGraph + _nomFile + ".dot -o " + pathGraph + _nomFile + ".svg").c_str()) != 0)
        {
            std::cerr << "Error lors de la génération du graphe." << std::endl;
        }

        // Supprimer le fichier .dot intermédiaire
        std::filesystem::remove(pathGraph + _nomFile + ".dot");
    }

    LlvmSerializer serializer(_context->getBackend()->getModule());
    serializer.SauvegarderCodeLLVM(pathProgramme + _nomFile + ".ll"); 

    // Restaurer le répertoire courant pour les includes au même niveau
    _repertoireCourant = _ancienRepertoire;
}

auto UnitCompilation::getChemin() const -> std::string
{
    return _cheminFileOriginal;
}