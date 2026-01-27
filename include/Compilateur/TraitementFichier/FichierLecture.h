#include "Compilateur/TraitementFichier/Interfaces/IEntre.h"
#include <fstream>

class FichierLecture : public IEntre 
{
    private: 
         std::string _path;
         std::ifstream _fichier;
    public: 
        std::string entrer() override;
        FichierLecture(const std::string& path);
        ~FichierLecture();  
};