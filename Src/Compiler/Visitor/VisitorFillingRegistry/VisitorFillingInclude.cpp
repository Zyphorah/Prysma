#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/Utils/OrchestratorInclude/OrchestratorInclude.h" 
#include "Compiler/AST/AST_Genere.h" 
#include <filesystem>

void FillingVisitorRegistry::visiter(NodeInclude* nodeInclude)
{
    std::filesystem::path parentFilePath(_contextGenCode->getCurrentFilePath());
    std::filesystem::path parentDirectory = parentFilePath.parent_path();
    std::filesystem::path absolutePath = parentDirectory / nodeInclude->getPath();
    _orchestrator->includeFile(absolutePath.string());
}

