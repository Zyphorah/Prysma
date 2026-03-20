#ifndef A43E134A_2FA4_4057_8CD2_CF059CEEA409
#define A43E134A_2FA4_4057_8CD2_CF059CEEA409

#include <set>
#include <string>
#include <mutex>
#include <vector>

class RegistreFichier {
public:
    void ajouterFichier(const std::string& cheminFichier);
    auto verifierFichier(const std::string& cheminFichier) -> bool;
    auto obtenirTousLesFichiers() -> std::vector<std::string>;

private:
    std::set<std::string> fichiersTraites;
    std::mutex mtx;
};

#endif /* A43E134A_2FA4_4057_8CD2_CF059CEEA409 */
