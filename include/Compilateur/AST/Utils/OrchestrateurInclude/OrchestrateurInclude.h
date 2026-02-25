#ifndef D2577958_A8A8_4878_AFA0_2B3478129911
#define D2577958_A8A8_4878_AFA0_2B3478129911

#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "Compilateur/Registre/RegistreFichier.h"
#include <string>
#include <tuple>

struct ContextGenCode;
class INoeud;

class OrchestrateurInclude
{
private: 
   FacadeConfigurationEnvironnement* _facadeConfigurationEnvironnement;
   RegistreFonctionGlobale* _registreFonctionGlobale;
   RegistreFichier* _registreFichier;
   std::string _repertoireCourant;
   std::vector<std::unique_ptr<FacadeConfigurationEnvironnement>> _facadesEnfants;

public:
    OrchestrateurInclude(FacadeConfigurationEnvironnement* facadeConfigurationEnvironnement, RegistreFonctionGlobale* registreFonctionGlobale, RegistreFichier* registreFichier);
    ~OrchestrateurInclude();

   void nouvelleInstance(const std::string& cheminFichier);
   void inclureFichier(const std::string& cheminAbsolu);
   void attendreFinPasse1();

private:
    std::tuple<ContextGenCode*, INoeud*, std::string, std::string> passe1(const std::string& cheminFichier);
    void passe2(ContextGenCode* context, INoeud* arbre, const std::string& nomFichier, const std::string& ancienRepertoire);
};

#endif /* D2577958_A8A8_4878_AFA0_2B3478129911 */
