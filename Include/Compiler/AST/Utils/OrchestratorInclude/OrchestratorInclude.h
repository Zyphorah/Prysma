#ifndef D2577958_A8A8_4878_AFA0_2B3478129911
#define D2577958_A8A8_4878_AFA0_2B3478129911

#include "Compiler/AST/Utils/OrchestratorInclude/UnitCompilation.h"
#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <set>
#include <memory>
#include <llvm-18/llvm/Support/ThreadPool.h>

struct ContextGenCode;
class INode;
class RegistryFunctionGlobale;
class RegistryFile;

class OrchestratorInclude
{
private: 
   std::mutex* _mutex;
   RegistryFunctionGlobale* _registryFunctionGlobale;
   RegistryFile* _registryFile;
   llvm::ThreadPool _threads;
   std::vector<std::unique_ptr<UnitCompilation>> _unitsCompilation;
   
   // Un registry dédié aux fichiers sources (.p)
   std::set<std::string> _fichiersDejaInclus; 

   // Indicateur d'error thread-safe pour savoir si la compilation a échoué
   std::atomic<bool> _aDesErrors{false};

   // Flag pour activer la génération du graphe AST
   bool _activerGraphViz;

public:
    OrchestratorInclude(RegistryFunctionGlobale* registryFunctionGlobale, RegistryFile* registryFile, std::mutex* mutex, bool activerGraphViz = false);
    ~OrchestratorInclude();

    OrchestratorInclude(const OrchestratorInclude&) = delete;
    auto operator=(const OrchestratorInclude&) -> OrchestratorInclude& = delete;
    OrchestratorInclude(OrchestratorInclude&&) = delete;
    auto operator=(OrchestratorInclude&&) -> OrchestratorInclude& = delete;
    
   void compilerProject(const std::string& cheminFile);
   void inclureFile(const std::string& cheminAbsolu);
   static void attendreFinPass(llvm::ThreadPool& threads);
   [[nodiscard]] auto aDesErrors() const -> bool;
   [[nodiscard]] auto estGraphVizActif() const -> bool;
};

#endif /* D2577958_A8A8_4878_AFA0_2B3478129911 */