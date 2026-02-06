#ifndef B7A1B053_311E_4DDE_9808_6B5699644912
#define B7A1B053_311E_4DDE_9808_6B5699644912
#include <string>

class ConstructeurSysteme
{
private:
    std::string _pathLib; 
    static std::string parcourirEtCollecterFichiers(const std::string& repertoire, const std::string& extension);

public:
    explicit ConstructeurSysteme(std::string pathLib);
    ~ConstructeurSysteme();

    static void lierLibExecutable();
    void compilerLib();
};

#endif /* B7A1B053_311E_4DDE_9808_6B5699644912 */
