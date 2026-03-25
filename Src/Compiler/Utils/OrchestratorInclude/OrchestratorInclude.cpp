#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Utils/OrchestratorInclude/OrchestratorInclude.h"
#include "Compiler/AST/Utils/OrchestratorInclude/UnitCompilation.h"
#include "Compiler/ManagerError.h"
#include "Compiler/FileProcessing/FileReading.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include <iostream>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <cstdlib>

#include <llvm-18/llvm/Support/ThreadPool.h>
#include <memory>
#include <mutex>
#include <string>


OrchestratorInclude::OrchestratorInclude(RegistryFunctionGlobale* registryFunctionGlobale, RegistryFile* registryFile, std::mutex* mutex, bool activerGraphViz)
    : _mutex(mutex), _registryFunctionGlobale(registryFunctionGlobale), _registryFile(registryFile), _activerGraphViz(activerGraphViz)
{}

OrchestratorInclude::~OrchestratorInclude()
= default;

auto OrchestratorInclude::estGraphVizActif() const -> bool {
    return _activerGraphViz;
}

auto OrchestratorInclude::aDesErrors() const -> bool {
    return _aDesErrors;
}

void OrchestratorInclude::compilerProject(const std::string& cheminFile) 
{
    inclureFile(cheminFile);
    _threads.wait(); // Attendre que tous les threads finissent la passe 1 


    for(const auto& unit : _unitsCompilation) {
        UnitCompilation* ptrUnit = unit.get();
        _threads.async([this, ptrUnit] {
            try {
                ptrUnit->passe2();
            } catch (const ErrorCompilation& e) {
                std::lock_guard<std::mutex> lock(*_mutex);
                _aDesErrors = true;
                std::cerr << "Error dans le fichier '" << ptrUnit->getChemin() << "': " << e.what() << std::endl;
            }
        });
    }
    _threads.wait(); // Attendre que tous les threads finissent la passe 2
}

void OrchestratorInclude::inclureFile(const std::string& cheminAbsolu)
{
    std::lock_guard<std::mutex> lock(*_mutex);
    if (_fichiersDejaInclus.count(cheminAbsolu) > 0) {
        return; // Le fichier a déjà été inclus, on ignore pour éviter les inclusions multiples
    }
    _fichiersDejaInclus.insert(cheminAbsolu);

   // On remplis le vecteur d'unité de compilation 
    _unitsCompilation.push_back(std::make_unique<UnitCompilation>(this, _registryFile, cheminAbsolu, _registryFunctionGlobale));
    auto *ptrUnitNouvelle = _unitsCompilation.back().get();

    // Filling du pool de thread 
    _threads.async([this, ptrUnitNouvelle, cheminAbsolu] {
        try {
            ptrUnitNouvelle->passe1();
        } catch (const ErrorCompilation& e) {
            std::lock_guard<std::mutex> lock(*_mutex);
            _aDesErrors = true;
            std::cerr << "Error dans le fichier '" << cheminAbsolu << "': " << e.what() << std::endl;
        }
    });

}
