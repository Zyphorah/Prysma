//===-- builder_systeme.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/file_processing/builder_systeme.h"
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

BuilderSystem::BuilderSystem(BuilderParams params)
    : _libPath(std::move(params.libPath)), 
      _libObjDir(std::move(params.libObjDir)),
      _buildDir(std::move(params.buildDir)), 
      _outputLL(std::move(params.outputLL)), 
      _executable(std::move(params.executable)) 
{}

BuilderSystem::~BuilderSystem() = default;

// Traverse and collect files with a given extension
auto BuilderSystem::traverseAndCollectFiles(const std::filesystem::path& directory, const std::string& extension) -> std::vector<std::string>
{
    std::vector<std::string> files;
    if (!fs::exists(directory)) {
        return files;
    }
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// Compile the library
void BuilderSystem::compileLib()
{
    if (!fs::exists(_libObjDir)) {
        fs::create_directories(_libObjDir);
    }

    std::vector<std::string> cppFiles = BuilderSystem::traverseAndCollectFiles(_libPath, ".cpp");
    for (const auto& file : cppFiles) {
        const fs::path filePath(file);
        std::string objectFile = (fs::path(_libObjDir) / filePath.filename()).replace_extension(".o").string();
        
        std::vector<std::string> cmdArgs = {"clang++", "-c", file, "-o", objectFile};
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
                std::cerr << "Error during library compilation." << std::endl;
            }
        } else {
            std::cerr << "Error during execution of clang++." << std::endl;
        }
    }
}

// Link the library to the executable
void BuilderSystem::linkLibExecutable()
{
    std::vector<std::string> objectFilesVec = BuilderSystem::traverseAndCollectFiles(_libObjDir, ".o");

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
            std::cerr << "Error during linking library to executable." << std::endl;
        }
    } else {
        std::cerr << "Error during execution of clang++." << std::endl;
    }
}
