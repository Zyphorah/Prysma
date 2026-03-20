#include "Compilateur/TraitementFichier/ConstructeurSysteme.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

ConstructeurSysteme::ConstructeurSysteme(std::string pathLib, std::string libObjDir, std::string buildDir, std::vector<std::string> outputLL, std::string executable) 
    : _pathLib(std::move(pathLib)), 
      _libObjDir(std::move(libObjDir)),
      _buildDir(std::move(buildDir)), 
      _outputLL(std::move(outputLL)), 
      _executable(std::move(executable)) 
{}

ConstructeurSysteme::~ConstructeurSysteme() = default;

auto ConstructeurSysteme::parcourirEtCollecterFichiers(const std::string& repertoire, const std::string& extension) -> std::vector<std::string> // NOLINT(bugprone-easily-swappable-parameters)
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
        std::vector<std::string> args = {"clang++", "-c", fichier, "-o", objectFile};
        std::string command;
        for (const auto& arg : args) {
            command += arg + " ";
        }
        int ret = std::system(command.c_str()); // NOLINT(cert-env33-c, concurrency-mt-unsafe)
        if (ret != 0) {
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
    std::vector<std::string> args = {"clang++"};
    if (!llFiles.empty()) { args.push_back(llFiles);}
    if (!objectFiles.empty()) { args.push_back(objectFiles);}
    args.emplace_back("-o");
    args.push_back((fs::path(_buildDir) / _executable).string());
    std::string command;
    for (const auto& arg : args) {
        command += arg + " ";
    }

    FILE* pipe = popen(command.c_str(), "r"); // NOLINT(cert-env33-c, concurrency-mt-unsafe)
    if (pipe == nullptr) {
        std::cerr << "Erreur lors de la liaison de la lib à l'exécutable." << std::endl;
    } else {
        int ret = pclose(pipe);
        if (ret != 0) {
            std::cerr << "Erreur lors de la liaison de la lib à l'exécutable." << std::endl;
        }
        }
    }
}