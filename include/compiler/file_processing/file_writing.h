//===-- file_writing.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef D554AA27_6112_4847_8679_9DF99DA76B4A
#define D554AA27_6112_4847_8679_9DF99DA76B4A
#include "interfaces/i_output.h"
#include <string>
#include <fstream>

class FileWriting : public IOutput
{
private: 
    std::string _path;
    std::ofstream _file;

public:
    void output(const std::string& data) override; 

    explicit FileWriting(const std::string& path);

    ~FileWriting() override;

    FileWriting(const FileWriting&) = delete;
    auto operator=(const FileWriting&) -> FileWriting& = delete;
    FileWriting(FileWriting&&) = delete;
    auto operator=(FileWriting&&) -> FileWriting& = delete;
};

#endif /* D554AA27_6112_4847_8679_9DF99DA76B4A */
