#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>

namespace fs = std::filesystem;

ConstructeurSysteme::ConstructeurSysteme(std::string pathLib, std::string libObjDir, std::string outputLL, std::string executable) 
    : _pathLib(std::move(pathLib)), 
      _libObjDir(std::move(libObjDir)), 
      _outputLL(std::move(outputLL)), 
      _executable(std::move(executable)) 
{}

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
    if (!fs::exists(_libObjDir)) {
        fs::create_directory(_libObjDir);
    }

    std::string fichiersCpp = ConstructeurSysteme::parcourirEtCollecterFichiers(_pathLib, ".cpp");
    std::stringstream stream(fichiersCpp);
    std::string fichier;
    while (stream >> fichier) {
        const fs::path filePath(fichier);
        std::string objectFile = (fs::path(_libObjDir) / filePath.filename()).replace_extension(".o").string();
        std::string command = "clang++ -c " + fichier + " -o " + objectFile;
        (void)system(command.c_str());
    }
}

void ConstructeurSysteme::lierLibExecutable()
{
    std::string objectFiles = ConstructeurSysteme::parcourirEtCollecterFichiers(_libObjDir, ".o");

    std::string command = "clang++ " + _outputLL + objectFiles + " -o " + _executable;
    (void)system(command.c_str());
}
