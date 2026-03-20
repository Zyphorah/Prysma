#include "Compilateur/Registre/RegistreFichier.h"
#include <mutex>
#include <string>
#include <vector>

void RegistreFichier::ajouterFichier(const std::string& cheminFichier) {
    std::lock_guard<std::mutex> lock(mtx);
    fichiersTraites.insert(cheminFichier);
}

auto RegistreFichier::verifierFichier(const std::string& cheminFichier) -> bool {
    std::lock_guard<std::mutex> lock(mtx);
    return fichiersTraites.count(cheminFichier) > 0;
}

auto RegistreFichier::obtenirTousLesFichiers() -> std::vector<std::string> {
    std::lock_guard<std::mutex> lock(mtx);
    return std::vector<std::string>(fichiersTraites.begin(), fichiersTraites.end());
}
