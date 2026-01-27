#include "Compilateur/Lecture/Interfaces/IEntre.h"

class FichierLecture : public IEntre 
{
    private: 
         std :: string _path; 
    public: 
        std :: string entrer() override;
        FichierLecture(const std :: string &path)
        {
            _path = path; 
        };
        ~FichierLecture();  
};