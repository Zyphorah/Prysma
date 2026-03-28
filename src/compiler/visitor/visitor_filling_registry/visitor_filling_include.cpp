#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "compiler/ast/utils/orchestrator_include/orchestrator_include.h" 
#include "compiler/ast/ast_genere.h" 
#include <filesystem>

void FillingVisitorRegistry::visiter(NodeInclude* nodeInclude)
{
    std::filesystem::path parentFilePath(_contextGenCode->getCurrentFilePath());
    std::filesystem::path parentDirectory = parentFilePath.parent_path();
    std::filesystem::path absolutePath = parentDirectory / nodeInclude->getPath().value.str();
    _orchestrator->includeFile(absolutePath.string());
}

