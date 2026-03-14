#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

ConstructeurSysteme::ConstructeurSysteme(std::string pathLib, std::string libObjDir, std::string buildDir, std::vector<std::string> outputLL, std::string executable) 
    : _pathLib(std::move(pathLib)), 
      _libObjDir(std::move(libObjDir)),
      _buildDir(std::move(buildDir)), 
      _outputLL(std::move(outputLL)), 
      _executable(std::move(executable)) 
{}

ConstructeurSysteme::~ConstructeurSysteme() {}

std::vector<std::string> ConstructeurSysteme::parcourirEtCollecterFichiers(const std::string& repertoire, const std::string& extension)
{
    std::vector<std::string> fichiers;
    if (!fs::exists(repertoire)) {
        return fichiers;
    }
    for (const auto& entry : fs::directory_iterator(repertoire)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            fichiers.push_back(entry.path().string());
        }
    }
    return fichiers;
}

void ConstructeurSysteme::compilerLib()
{
    if (!fs::exists(_libObjDir)) {
        fs::create_directories(_libObjDir);
    }

    std::vector<std::string> fichiersCpp = ConstructeurSysteme::parcourirEtCollecterFichiers(_pathLib, ".cpp");
    for (const auto& fichier : fichiersCpp) {
        const fs::path filePath(fichier);
        std::string objectFile = (fs::path(_libObjDir) / filePath.filename()).replace_extension(".o").string();
        std::string command = "clang++ -c ";
        command += fichier;
        command += " -o ";
        command += objectFile;
        if (system(command.c_str()) != 0) {
            std::cerr << "Erreur lors de la compilation de la lib." << std::endl;
        }
    }
}

void ConstructeurSysteme::lierLibExecutable()
{
    std::vector<std::string> objectFilesVec = ConstructeurSysteme::parcourirEtCollecterFichiers(_libObjDir, ".o");
    std::string objectFiles;
    for(const auto& file : objectFilesVec) {
        objectFiles += " " + file;
    }

    std::string llFiles;
    for (const auto& llFile : _outputLL) {
        llFiles += " " + (fs::path(_buildDir) / llFile).string();
    }

    std::string command = "clang++ " + llFiles + objectFiles + " -o " + (fs::path(_buildDir) / _executable).string();
    if (system(command.c_str()) != 0) {
        std::cerr << "Erreur lors de la liaison de la lib à l'exécutable." << std::endl;
    }
}
