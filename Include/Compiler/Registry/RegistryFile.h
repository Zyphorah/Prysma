#ifndef A43E134A_2FA4_4057_8CD2_CF059CEEA409
#define A43E134A_2FA4_4057_8CD2_CF059CEEA409

#include <set>
#include <string>
#include <mutex>
#include <vector>

class RegistryFile {
public:
    void ajouterFile(const std::string& cheminFile);
    auto verifierFile(const std::string& cheminFile) -> bool;
    auto obtenirTousLesFiles() -> std::vector<std::string>;

private:
    std::set<std::string> fichiersTraites;
    std::mutex mtx;
};

#endif /* A43E134A_2FA4_4057_8CD2_CF059CEEA409 */
