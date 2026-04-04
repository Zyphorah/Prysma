//===-- file_writing.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/file_processing/file_writing.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

FileWriting::FileWriting(const std::string& path)
    : _path(path), _file(path, std::ios::out | std::ios::app)
{
}

FileWriting::~FileWriting()
{
    if (_file.is_open()) {
        _file.close();
    }
}

void FileWriting::output(const std::string& data)
{
    if (!_file) {
        throw std::runtime_error("Error: unable to open output.txt for writing\n");
    }
    _file << data;
    
    _file.close();
}
