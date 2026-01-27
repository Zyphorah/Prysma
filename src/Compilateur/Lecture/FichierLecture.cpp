#include "Compilateur/Lecture/FichierLecture.h"

std :: string FichierLecture :: entrer()
{
    std :: string ligne; 
    std :: string document; 

    std::ifstream fichier(_path, std::ios::in); 

    if(fichier.is_open())
    {
        while(std :: getline(fichier,ligne))
        {
            document += ligne; 
        }
        fichier.close();
        return document;
    }
    throw std::runtime_error("Impossible d'ouvrir le fichier !");
 
};

FichierLecture::~FichierLecture() {

}