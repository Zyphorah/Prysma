#include "Compiler/FileProcessing/Interfaces/IInput.h"
#include <fstream>
#include <string>

class FileReading : public IInput 
{
    private: 
         std::string _path;
         std::ifstream _fichier;
    public: 
        auto inputr() -> std::string override;
        explicit FileReading(const std::string& path);
        ~FileReading() override;  

        FileReading(const FileReading&) = delete;
        auto operator=(const FileReading&) -> FileReading& = delete;
        FileReading(FileReading&&) = delete;
        auto operator=(FileReading&&) -> FileReading& = delete;
};