#include "Compilateur/TraitementFichier/Interfaces/IEntre.h"
#include <fstream>
#include <string>

class FichierLecture : public IEntre 
{
    private: 
         std::string _path;
         std::ifstream _fichier;
    public: 
        auto entrer() -> std::string override;
        explicit FichierLecture(const std::string& path);
        ~FichierLecture() override;  

        FichierLecture(const FichierLecture&) = delete;
        auto operator=(const FichierLecture&) -> FichierLecture& = delete;
        FichierLecture(FichierLecture&&) = delete;
        auto operator=(FichierLecture&&) -> FichierLecture& = delete;
};