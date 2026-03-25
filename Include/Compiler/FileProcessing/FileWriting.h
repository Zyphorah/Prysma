#ifndef D554AA27_6112_4847_8679_9DF99DA76B4A
#define D554AA27_6112_4847_8679_9DF99DA76B4A
#include "Interfaces/IOutput.h"
#include <string>
#include <fstream>

class FileWriting : public IOutput
{
    private: 
        std::string _path;
        std::ofstream _fichier;

    public:
        void sort(const std::string& data) override; 

        explicit FileWriting(const std::string& path);

        ~FileWriting() override;

        FileWriting(const FileWriting&) = delete;
        auto operator=(const FileWriting&) -> FileWriting& = delete;
        FileWriting(FileWriting&&) = delete;
        auto operator=(FileWriting&&) -> FileWriting& = delete;
};

#endif /* D554AA27_6112_4847_8679_9DF99DA76B4A */
