#ifndef B7A1B053_311E_4DDE_9808_6B5699644912
#define B7A1B053_311E_4DDE_9808_6B5699644912
#include <string>
#include <vector>

#include <filesystem>

class BuilderSysteme
{
private:
    std::string _pathLib;
    std::string _libObjDir;
    std::string _buildDir;
    std::vector<std::string> _outputLL;
    std::string _executable;
    static auto parcourirEtCollecterFiles(const std::filesystem::path& repertoire, const std::string& extension) -> std::vector<std::string>;

public:
    struct BuilderParams {
        std::string pathLib;
        std::string libObjDir;
        std::string buildDir;
        std::vector<std::string> outputLL;
        std::string executable;
    };

    explicit BuilderSysteme(BuilderParams params);
    ~BuilderSysteme();

    BuilderSysteme(const BuilderSysteme&) = default;
    auto operator=(const BuilderSysteme&) -> BuilderSysteme& = default;
    BuilderSysteme(BuilderSysteme&&) = default;
    auto operator=(BuilderSysteme&&) -> BuilderSysteme& = default;
    
    void lierLibExecutable();
    void compilerLib();
};

#endif /* B7A1B053_311E_4DDE_9808_6B5699644912 */
