//===-- registry_file.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef A43E134A_2FA4_4057_8CD2_CF059CEEA409
#define A43E134A_2FA4_4057_8CD2_CF059CEEA409

#include <set>
#include <string>
#include <mutex>
#include <vector>

class FileRegistry {
public:
    void addFile(const std::string& filePath);
    auto checkFile(const std::string& filePath) -> bool;
    auto getAllFiles() -> std::vector<std::string>;

private:
    std::set<std::string> processedFiles;
    std::mutex mtx;
};

#endif /* A43E134A_2FA4_4057_8CD2_CF059CEEA409 */
