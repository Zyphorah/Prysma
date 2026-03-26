#include "Compiler/FileProcessing/FileWriting.h"
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