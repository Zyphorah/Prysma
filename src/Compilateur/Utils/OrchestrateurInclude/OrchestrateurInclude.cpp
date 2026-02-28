#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/OrchestrateurInclude.h"
#include "Compilateur/AST/Utils/OrchestrateurInclude/UniteCompilation.h"
#include "Compilateur/TraitementFichier/FichierLecture.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Visiteur/VisiteurRemplissageRegistre/VisiteurRemplissageRegistre.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>

OrchestrateurInclude::OrchestrateurInclude(RegistreFonctionGlobale* registreFonctionGlobale, RegistreFichier* registreFichier, std::mutex* mutex, bool activerGraphViz)
    : _mutex(mutex), _registreFonctionGlobale(registreFonctionGlobale), _registreFichier(registreFichier), _activerGraphViz(activerGraphViz)
{}

OrchestrateurInclude::~OrchestrateurInclude()
{}

void OrchestrateurInclude::attendreFinPass(std::vector<std::thread>& threads) {
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void OrchestrateurInclude::compilerProjet(const std::string& cheminFichier) 
{
    inclureFichier(cheminFichier);
    // Obligatoire d'utiliser ça pour éviter un race condition entre les thread de la passe 1 et la passe 2 
    size_t index = 0;
    while (true) {
        std::thread threadExtrait;
        {
            std::lock_guard<std::mutex> guard(*_mutex); 
            if (index >= _threads.size()) {
                break; 
            }
            threadExtrait = std::move(_threads[index]);
        } 
        
        if (threadExtrait.joinable()) {
            threadExtrait.join();
        }
        index++;
    }
    // Il faut netoyer l'équipe de la passe un
    // Car après le travail des threads on ne peux pas relancer un thread mort on dois en refaire des nouveaux pour la passe 2 
    _threads.clear();

    // parcourir le vecteur d'unité de compilation thread pour faire la passe 2 en parallèle
    std::vector<std::thread> threadsPasse2;
    threadsPasse2.reserve(_unitesCompilation.size());
    for (const auto& unite : _unitesCompilation) {
        threadsPasse2.emplace_back([this, unite = unite.get()]() {
            try
            {
                unite->passe2();
            }
            catch (const std::exception& e)
            {
                std::string nomFichier = std::filesystem::path(unite->getChemin()).filename().string();
                std::cerr << "Erreur (Passe 2) dans le fichier " << nomFichier << " : " << e.what() << std::endl;
                _aDesErreurs = true;
            }
        });
    }
    attendreFinPass(threadsPasse2);
}

void OrchestrateurInclude::inclureFichier(const std::string& cheminAbsolu)
{
    std::lock_guard<std::mutex> guard(*_mutex);

    // Vérifier si le fichier est déjà compilé par un thread pour éviter les problème de race condition
    if(_registreFichier->verifierFichier(cheminAbsolu)) {
        return;
    }

    auto* uniteCompilation = new UniteCompilation(this, _registreFichier, cheminAbsolu, _registreFonctionGlobale);
    _unitesCompilation.push_back(std::unique_ptr<UniteCompilation>(uniteCompilation));

    // Lancer un thread pour la passe 1 : ce sont les thread que nous allons utilisée pour faire le traitement en parallèle des différents include
    // Je vais avoir un vecteur de thread utilisable pour faire le travail 

    std::thread threadCourent = std::thread([this, uniteCompilation, cheminAbsolu]() {
        try {
            uniteCompilation->passe1();
        } catch (const std::exception& e) {
          std::string nomFichier = std::filesystem::path(cheminAbsolu).filename().string();
          std::cerr << "Erreur lors de la compilation du fichier " + nomFichier + " : " + e.what() << std::endl;
          _aDesErreurs = true;
        }
    });
   
    // Le stocker dans un vecteur pour pouvoir faire un join à la fin de la compilation pour attendre que tout les thread de la passe 1 soit terminée
    _threads.push_back(std::move(threadCourent)); 
}

bool OrchestrateurInclude::aDesErreurs() const
{
    return _aDesErreurs.load();
}

bool OrchestrateurInclude::estGraphVizActif() const
{
    return _activerGraphViz;
}
