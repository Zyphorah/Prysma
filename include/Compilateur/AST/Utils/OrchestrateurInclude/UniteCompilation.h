#ifndef DB7C496D_6A43_4B78_B490_52A0C21C5224
#define DB7C496D_6A43_4B78_B490_52A0C21C5224

#include "Compilateur/AST/Utils/OrchestrateurInclude/FacadeConfigurationEnvironnement.h"
#include "FacadeConfigurationEnvironnement.h"
#include <string>
#include <memory>

class OrchestrateurInclude; 
struct ContextGenCode;
class INoeud;
class RegistreFichier;
class RegistreFonctionGlobale;

class UniteCompilation
{
private: 

    // Groupe de donnée globale pour l'unité de compilation 
    // Orchestrateur récursif pour les includes, utilise un système de mutex pour protéger les données partagées 
    OrchestrateurInclude* _orchestrateur;

    // contexte séparé pour chaque unité de compilation pour éviter les conflits entre les includes imbriqués
    // Note: ne jamais partager toujours copier c'est donnée pour ne pas avoir de problème de race condition
    std::unique_ptr<FacadeConfigurationEnvironnement> _facadeConfigurationEnvironnement;
    std::string _nomFichier;
    
    std::string _ancienRepertoire;
    std::string _repertoireCourant;

    RegistreFichier* _registreFichier;
    ContextGenCode* _context;
    INoeud* _arbre;
    std::string _cheminFichierOriginal;

public: 
    UniteCompilation(OrchestrateurInclude* orchestrateur, RegistreFichier* registre, std::string cheminFichier, RegistreFonctionGlobale* registreFonctionGlobale);
    ~UniteCompilation();

    // Delete copy and move constructors and assignment operators
    UniteCompilation(const UniteCompilation&) = delete;
    auto operator=(const UniteCompilation&) -> UniteCompilation& = delete;
    UniteCompilation(UniteCompilation&&) = delete;
    auto operator=(UniteCompilation&&) -> UniteCompilation& = delete;

    void passe1();
    // Appelée après que tous les threads passe1 soient terminés.
    // Remplit les registres locaux (fonction, variable) à partir du registre global complet.
    void passe2();

    [[nodiscard]] auto getChemin() const -> std::string;
};

#endif /* DB7C496D_6A43_4B78_B490_52A0C21C5224 */