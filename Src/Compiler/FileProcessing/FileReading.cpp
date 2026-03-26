#include "Compiler/FileProcessing/FileReading.h"
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