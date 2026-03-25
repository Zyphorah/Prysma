#include "Compiler/AST/Utils/OrchestratorInclude/OrchestratorInclude.h"
#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"

FillingVisitorRegistry::FillingVisitorRegistry(ContextGenCode* contextGenCode, OrchestratorInclude* orchestrator) 
    : _contextGenCode(contextGenCode), _orchestrator(orchestrator) {}

FillingVisitorRegistry::~FillingVisitorRegistry() = default;
