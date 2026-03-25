#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/Utils/OrchestratorInclude/OrchestratorInclude.h" 
#include "Compiler/AST/AST_Genere.h" 
#include <filesystem>

void FillingVisitorRegistry::visiter(NodeInclude* nodeInclude)
{
    std::filesystem::path cheminFileParent(_contextGenCode->getCheminFileCourant());
    std::filesystem::path repertoireParent = cheminFileParent.parent_path();
    std::filesystem::path cheminAbsolu = repertoireParent / nodeInclude->getPath();
    _orchestrator->inclureFile(cheminAbsolu.string());
}

