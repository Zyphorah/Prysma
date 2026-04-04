//===-- registry_file.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/registry/registry_file.h"
#include <mutex>
#include <string>
#include <vector>

void FileRegistry::addFile(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(mtx);
    processedFiles.insert(filePath);
}

auto FileRegistry::checkFile(const std::string& filePath) -> bool {
    std::lock_guard<std::mutex> lock(mtx);
    return processedFiles.count(filePath) > 0;
}

auto FileRegistry::getAllFiles() -> std::vector<std::string> {
    std::lock_guard<std::mutex> lock(mtx);
    return std::vector<std::string>(processedFiles.begin(), processedFiles.end());
}
