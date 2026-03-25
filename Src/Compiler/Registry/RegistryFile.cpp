#include "Compiler/Registry/RegistryFile.h"
#include <mutex>
#include <string>
#include <vector>

void RegistryFile::ajouterFile(const std::string& cheminFile) {
    std::lock_guard<std::mutex> lock(mtx);
    fichiersTraites.insert(cheminFile);
}

auto RegistryFile::verifierFile(const std::string& cheminFile) -> bool {
    std::lock_guard<std::mutex> lock(mtx);
    return fichiersTraites.count(cheminFile) > 0;
}

auto RegistryFile::obtenirTousLesFiles() -> std::vector<std::string> {
    std::lock_guard<std::mutex> lock(mtx);
    return std::vector<std::string>(fichiersTraites.begin(), fichiersTraites.end());
}
