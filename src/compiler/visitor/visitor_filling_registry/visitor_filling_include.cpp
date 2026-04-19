#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "compiler/ast/utils/orchestrator_include/orchestrator_include.h" 
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt" 
#include <filesystem>
#include <string>

void FillingVisitorRegistry::visiter(NodeInclude* nodeInclude)
{
    std::filesystem::path parentFilePath(_contextGenCode->getCurrentFilePath());
    std::filesystem::path parentDirectory = parentFilePath.parent_path();
    std::filesystem::path absolutePath = parentDirectory / std::string(nodeInclude->getPath().value);

    std::filesystem::path absolutePath2 = parentDirectory / std::string(nodeInclude->getPath().value);

    _orchestrator->includeFile(absolutePath.string());
}

