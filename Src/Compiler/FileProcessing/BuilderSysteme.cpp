#include "Compiler/FileProcessing/BuilderSysteme.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <spawn.h>
#include <sys/wait.h>


namespace fs = std::filesystem;

BuilderSysteme::BuilderSysteme(BuilderParams params)
    : _pathLib(std::move(params.pathLib)), 
      _libObjDir(std::move(params.libObjDir)),
      _buildDir(std::move(params.buildDir)), 
      _outputLL(std::move(params.outputLL)), 
      _executable(std::move(params.executable)) 
{}

BuilderSysteme::~BuilderSysteme() = default;

auto BuilderSysteme::parcourirEtCollecterFiles(const std::filesystem::path& repertoire, const std::string& extension) -> std::vector<std::string>
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

void BuilderSysteme::compilerLib()
{
    if (!fs::exists(_libObjDir)) {
        fs::create_directories(_libObjDir);
    }

    std::vector<std::string> fichiersCpp = BuilderSysteme::parcourirEtCollecterFiles(_pathLib, ".cpp");
    for (const auto& fichier : fichiersCpp) {
        const fs::path filePath(fichier);
        std::string objectFile = (fs::path(_libObjDir) / filePath.filename()).replace_extension(".o").string();
        
        std::vector<std::string> cmdArgs = {"clang++", "-c", fichier, "-o", objectFile};
        std::vector<char*> argv;
        argv.reserve(cmdArgs.size() + 1);
        for (auto& arg : cmdArgs) {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr);

        pid_t pid = 0;
        int status = 0;
        if (posix_spawnp(&pid, "clang++", nullptr, nullptr, argv.data(), environ) == 0) {
            waitpid(pid, &status, 0);
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                std::cerr << "Error lors de la compilation de la lib." << std::endl;
            }
        } else {
            std::cerr << "Error lors de l'exécution de clang++." << std::endl;
        }
    }
}

void BuilderSysteme::lierLibExecutable()
{
    std::vector<std::string> objectFilesVec = BuilderSysteme::parcourirEtCollecterFiles(_libObjDir, ".o");

    std::vector<std::string> args = {"clang++"};
    for (const auto& llFile : _outputLL) {
        args.push_back((fs::path(_buildDir) / llFile).string());
    }
    for(const auto& file : objectFilesVec) {
        args.push_back(file);
    }
    
    args.emplace_back("-o");
    args.push_back((fs::path(_buildDir) / _executable).string());
    
    std::vector<char*> argv;
    argv.reserve(args.size() + 1);
    for (auto& arg : args) {
        argv.push_back(arg.data());
    }
    argv.push_back(nullptr);

    pid_t pid = 0;
    int status = 0;
    if (posix_spawnp(&pid, "clang++", nullptr, nullptr, argv.data(), environ) == 0) {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cerr << "Error lors de la liaison de la lib à l'exécutable." << std::endl;
        }
    } else {
        std::cerr << "Error lors de l'exécution de clang++." << std::endl;
    }
}