#include "Compilateur/TraitementFichier/FichierEcriture.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

FichierEcriture::FichierEcriture(const std::string& path)
    : _path(path), _fichier(path, std::ios::out | std::ios::app)
{
}

void FichierEcriture::sort(const std::string& data)
{
    if (!_fichier) {
        throw std::runtime_error("Erreur : impossible d'ouvrir sortie.txt pour écriture\n");
        return;
    }
    _fichier << data;
    
    _fichier.close();
}