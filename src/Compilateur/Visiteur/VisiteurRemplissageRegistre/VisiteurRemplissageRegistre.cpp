#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"

VisiteurRemplissageRegistre::VisiteurRemplissageRegistre(ContextGenCode* contextGenCode, OrchestrateurInclude* orchestrateur) 
    : _contextGenCode(contextGenCode), _orchestrateur(orchestrateur) {}

VisiteurRemplissageRegistre::~VisiteurRemplissageRegistre() = default;
