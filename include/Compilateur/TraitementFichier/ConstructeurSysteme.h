#ifndef B7A1B053_311E_4DDE_9808_6B5699644912
#define B7A1B053_311E_4DDE_9808_6B5699644912
#include <string>
#include <vector>

class ConstructeurSysteme
{
private:
    std::string _pathLib;
    std::string _libObjDir;
    std::string _buildDir;
    std::vector<std::string> _outputLL;
    std::string _executable;
    static auto parcourirEtCollecterFichiers(const std::string& repertoire, const std::string& extension) -> std::vector<std::string>;

public:
    ConstructeurSysteme(std::string pathLib, std::string libObjDir, std::string buildDir, std::vector<std::string> outputLL, std::string executable);
    ~ConstructeurSysteme();

    ConstructeurSysteme(const ConstructeurSysteme&) = default;
    auto operator=(const ConstructeurSysteme&) -> ConstructeurSysteme& = default;
    ConstructeurSysteme(ConstructeurSysteme&&) = default;
    auto operator=(ConstructeurSysteme&&) -> ConstructeurSysteme& = default;
    
    void lierLibExecutable();
    void compilerLib();
};

#endif /* B7A1B053_311E_4DDE_9808_6B5699644912 */
