#include "Compilateur/TraitementFichier/FichierLecture.h"
#include <ios>
#include <stdexcept>
#include <string>

FichierLecture::FichierLecture(const std::string& path)
    : _path(path), _fichier(path, std::ios::in)
{
}

std::string FichierLecture :: entrer()
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
    throw std::runtime_error("Impossible d'ouvrir le fichier !");
 
};

FichierLecture::~FichierLecture() = default;