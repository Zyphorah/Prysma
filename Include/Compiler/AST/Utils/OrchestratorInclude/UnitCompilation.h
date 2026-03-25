#ifndef DB7C496D_6A43_4B78_B490_52A0C21C5224
#define DB7C496D_6A43_4B78_B490_52A0C21C5224

#include "Compiler/AST/Utils/OrchestratorInclude/ConfigurationFacadeEnvironment.h"
#include "ConfigurationFacadeEnvironment.h"
#include <string>
#include <memory>

class OrchestratorInclude; 
struct ContextGenCode;
class INode;
class RegistryFile;
class RegistryFunctionGlobale;

class UnitCompilation
{
private: 

    // Groupe de donnée globale pour l'unité de compilation 
    // Orchestrator récursif pour les includes, utilise un système de mutex pour protéger les données partagées 
    OrchestratorInclude* _orchestrator;

    // contexte séparé pour chaque unité de compilation pour éviter les conflits input les includes imbriqués
    // Note: ne jamais partager toujours copier c'est donnée pour ne pas avoir de problème de race condition
    std::unique_ptr<ConfigurationFacadeEnvironnement> _facadeConfigurationEnvironnement;
    std::string _nomFile;
    
    std::string _ancienRepertoire;
    std::string _repertoireCourant;

    RegistryFile* _registryFile;
    ContextGenCode* _context;
    INode* _tree;
    std::string _cheminFileOriginal;

public: 
    UnitCompilation(OrchestratorInclude* orchestrator, RegistryFile* registry, std::string cheminFile, RegistryFunctionGlobale* registryFunctionGlobale);
    ~UnitCompilation();

    UnitCompilation(const UnitCompilation&) = delete;
    auto operator=(const UnitCompilation&) -> UnitCompilation& = delete;
    UnitCompilation(UnitCompilation&&) = delete;
    auto operator=(UnitCompilation&&) -> UnitCompilation& = delete;

    void passe1();
    // Callée après que tous les threads passe1 soient terminés.
    // Remplit les registrys locaux (function, variable) à partir du registry global complet.
    void passe2();

    [[nodiscard]] auto getChemin() const -> std::string;
};

#endif /* DB7C496D_6A43_4B78_B490_52A0C21C5224 */