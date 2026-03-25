#include "Compiler/FileProcessing/FileWriting.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

FileWriting::FileWriting(const std::string& path)
    : _path(path), _fichier(path, std::ios::out | std::ios::app)
{
}

FileWriting::~FileWriting()
{
    if (_fichier.is_open()) {
        _fichier.close();
    }
}

void FileWriting::sort(const std::string& data)
{
    if (!_fichier) {
        throw std::runtime_error("Error : impossible d'ouvrir output.txt pour écriture\n");
    }
    _fichier << data;
    
    _fichier.close();
}