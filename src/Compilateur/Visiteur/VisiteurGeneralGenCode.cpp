#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"


VisiteurGeneralGenCode::VisiteurGeneralGenCode(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registre)
{
    _backend = std::move(backend);
    _registre = std::move(registre);
}

VisiteurGeneralGenCode::~VisiteurGeneralGenCode()
{}