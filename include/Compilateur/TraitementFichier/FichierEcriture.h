#ifndef D554AA27_6112_4847_8679_9DF99DA76B4A
#define D554AA27_6112_4847_8679_9DF99DA76B4A
#include "Interfaces/ISortie.h"
#include <string>
#include <fstream>

class FichierEcriture : public ISortie
{
    private: 
        std::string _path;
        std::ofstream _fichier;

    public:
        void sort(const std::string& data) override; 

        explicit FichierEcriture(const std::string& path);

        ~FichierEcriture() override;

        FichierEcriture(const FichierEcriture&) = delete;
        auto operator=(const FichierEcriture&) -> FichierEcriture& = delete;
        FichierEcriture(FichierEcriture&&) = delete;
        auto operator=(FichierEcriture&&) -> FichierEcriture& = delete;
};

#endif /* D554AA27_6112_4847_8679_9DF99DA76B4A */
