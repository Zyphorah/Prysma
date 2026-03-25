#include "Compiler/FileProcessing/FileReading.h"
#include <ios>
#include <stdexcept>
#include <string>

FileReading::FileReading(const std::string& path)
    : _path(path), _fichier(path, std::ios::in)
{
}

std::string FileReading :: inputr()
{
    std::string ligne; 
    std::string document; 

    if(_fichier.is_open())
    {
        while(std::getline(_fichier,ligne))
        {
            document += ligne;
            document += "\n";  
        }
        _fichier.close();
        return document;
    }
    throw std::runtime_error("Impossible d'ouvrir le fichier : " + _path);
 
};

FileReading::~FileReading() = default;