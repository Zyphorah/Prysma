//===-- file_reading.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/file_processing/file_reading.h"
#include <ios>
#include <stdexcept>
#include <string>

FileReading::FileReading(const std::string& path)
    : _path(path), _file(path, std::ios::in)
{
}

auto FileReading::getInput() -> std::string
{
    std::string line; 
    std::string document; 

    if(_file.is_open())
    {
        while(std::getline(_file, line))
        {
            document += line;
            document += "\n";  
        }
        _file.close();
        return document;
    }
    throw std::runtime_error("Unable to open file: " + _path);
};

FileReading::~FileReading() = default;
