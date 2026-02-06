#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>

namespace fs = std::filesystem;

ConstructeurSysteme::ConstructeurSysteme(std::string pathLib) : _pathLib(std::move(pathLib)) {}

ConstructeurSysteme::~ConstructeurSysteme() {}

std::string ConstructeurSysteme::parcourirEtCollecterFichiers(const std::string& repertoire, const std::string& extension)
{
    std::string fichiers;
    for (const auto& entry : fs::directory_iterator(repertoire)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            fichiers += " " + entry.path().string();
        }
    }
    return fichiers;
}

void ConstructeurSysteme::compilerLib()
{
    std::string libObjDir = "Lib";

    if (!fs::exists(libObjDir)) {
        fs::create_directory(libObjDir);
    }

    std::string fichiersCpp = ConstructeurSysteme::parcourirEtCollecterFichiers(_pathLib, ".cpp");
    std::stringstream stream(fichiersCpp);
    std::string fichier;
    while (stream >> fichier) {
        const fs::path filePath(fichier);
        std::string objectFile = (fs::path(libObjDir) / filePath.filename()).replace_extension(".o").string();
        std::string command = "clang++ -c " + fichier + " -o " + objectFile;
        std::cout << "Execution : " << command << std::endl;
        system(command.c_str());
    }
}

void ConstructeurSysteme::lierLibExecutable()
{
    std::string libObjDir = "Lib";
    std::string objectFiles = ConstructeurSysteme::parcourirEtCollecterFichiers(libObjDir, ".o");

    std::string outputLL = "output.ll";
    std::string executable = "programme";
    std::string command = "clang++ " + outputLL + objectFiles + " -o " + executable;
    std::cout << "Execution : " << command << std::endl;
    system(command.c_str());
}
