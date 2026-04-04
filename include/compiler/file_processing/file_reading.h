//===-- file_reading.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/file_processing/interfaces/i_input.h"
#include <fstream>
#include <string>

class FileReading : public IInput 
{
private: 
    std::string _path;
    std::ifstream _file;
public: 
    auto getInput() -> std::string override;
    explicit FileReading(const std::string& path);
    ~FileReading() override;  

    FileReading(const FileReading&) = delete;
    auto operator=(const FileReading&) -> FileReading& = delete;
    FileReading(FileReading&&) = delete;
    auto operator=(FileReading&&) -> FileReading& = delete;
};
