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

UnitCompilation::UnitCompilation(OrchestratorInclude* orchestrator, FileRegistry* registry, std::string filePath, RegistryFunctionGlobal* registryFunctionGlobale) 
    : _orchestrator(orchestrator), 
      _fileRegistry(registry),
      _context(nullptr),
      _tree(nullptr),
      _originalFilePath(std::move(filePath)) 
{
    // Initialization of the separate context (the private bubble)
    _facadeConfigurationEnvironment = std::make_unique<ConfigurationFacadeEnvironment>(registryFunctionGlobale, _fileRegistry);
}

UnitCompilation::~UnitCompilation() 
{
    _tree = nullptr; 
    _context = nullptr; 
}

void UnitCompilation::pass1() {
    std::filesystem::path absolutePath = std::filesystem::absolute(_originalFilePath);
    
    if (!std::filesystem::exists(absolutePath) && !_currentDirectory.empty()) {
        absolutePath = std::filesystem::path(_currentDirectory) / _originalFilePath;
    }
    
    std::string resolvedPath = absolutePath.string();
    
    _oldDirectory = _currentDirectory;
    _currentDirectory = absolutePath.parent_path().string();

    // Build a unique file name including the parent folder name 
    // to avoid collisions when multiple files have the same name in different folders
    // Ex: ParentA/ChildA.p -> "ParentA_ChildA", ParentB/ChildA.p -> "ParentB_ChildA"
    std::string baseName = absolutePath.stem().string();
    std::string parentName = absolutePath.parent_path().filename().string();
    _fileName = parentName + "_" + baseName;

    _fileRegistry->addFile("programme/" + _fileName + ".ll");

    _facadeConfigurationEnvironment->initialize(resolvedPath);

    _context = _facadeConfigurationEnvironment->getContext();
    BuilderTreeInstruction* builderTreeInstruction = _facadeConfigurationEnvironment->getBuilderTreeInstruction();

    FileReading fileReading(resolvedPath);
    std::string document = fileReading.getInput();

    std::vector<Token> tokens = Lexer::tokenize(document);

    _tree = builderTreeInstruction->build(tokens);

    FillingVisitorRegistry visitorFillingRegistry(_context, _orchestrator);
    _tree->accept(&visitorFillingRegistry);
    FillingVisitorBodyClass visitorFillingBodyClass(_context);
    _tree->accept(&visitorFillingBodyClass);
}

void UnitCompilation::pass2() {
    if (_tree == nullptr) 
    {
        return;
    }

    BuilderEnvironmentRegistryFunction builderEnvironmentRegistryFunction(_context);
    builderEnvironmentRegistryFunction.fill();

    BuilderEnvironmentRegistryVariable builderEnvironmentRegistryVariable(_context);
    builderEnvironmentRegistryVariable.fill();

    std::filesystem::path buildDir = std::filesystem::canonical("/proc/self/exe").parent_path();
    std::string pathProgram = (buildDir / "programme/").string();
    std::string pathGraph = (buildDir / "graphe/").string();

    GeneralVisitorGenCode visitor(_context, _orchestrator);
    _tree->accept(&visitor);

    if (_orchestrator->isGraphVizEnabled()) {
        OutputVisualGraphText outputVisualGraph(pathGraph + _fileName + ".dot");
        auto visitorGraphViz = std::make_unique<GeneralVisitorGraphViz>(std::move(outputVisualGraph));
        _tree->accept(visitorGraphViz.get());
        visitorGraphViz->generate();

        if (system(("dot -Tsvg " + pathGraph + _fileName + ".dot -o " + pathGraph + _fileName + ".svg").c_str()) != 0)
        {
            std::cerr << "Error during graph generation." << std::endl;
        }

        // Remove the intermediate .dot file
        std::filesystem::remove(pathGraph + _fileName + ".dot");
    }

    LlvmSerializer serializer(_context->getBackend()->getModule());
    serializer.SaveLLVMCode(pathProgram + _fileName + ".ll"); 

    // Restore the current directory for includes at the same level
    _currentDirectory = _oldDirectory;
}

auto UnitCompilation::getPath() const -> std::string
{
    return _originalFilePath;
}