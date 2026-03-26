#ifndef B7A1B053_311E_4DDE_9808_6B5699644912
#define B7A1B053_311E_4DDE_9808_6B5699644912
#include <string>
#include <vector>
#include <filesystem>

class BuilderSystem
{
private:
    std::string _libPath;
    std::string _libObjDir;
    std::string _buildDir;
    std::vector<std::string> _outputLL;
    std::string _executable;
    static auto traverseAndCollectFiles(const std::filesystem::path& directory, const std::string& extension) -> std::vector<std::string>;

public:
    struct BuilderParams {
        std::string libPath;
        std::string libObjDir;
        std::string buildDir;
        std::vector<std::string> outputLL;
        std::string executable;
    };

    explicit BuilderSystem(BuilderParams params);
    ~BuilderSystem();

    BuilderSystem(const BuilderSystem&) = default;
    auto operator=(const BuilderSystem&) -> BuilderSystem& = default;
    BuilderSystem(BuilderSystem&&) = default;
    auto operator=(BuilderSystem&&) -> BuilderSystem& = default;
    
    void linkLibExecutable();
    void compileLib();
};

#endif /* B7A1B053_311E_4DDE_9808_6B5699644912 */
