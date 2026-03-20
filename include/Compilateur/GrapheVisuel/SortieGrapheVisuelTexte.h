#ifndef ECD975BF_C219_4D88_AC4A_B12DE1902F2B
#define ECD975BF_C219_4D88_AC4A_B12DE1902F2B

#include <string>
#include <sstream>

class SortieGrapheVisuelTexte
{
private:    
    std::string _cheminSortie;
    std::stringstream _contenuGraphe;
    int _compteurId;
    auto obtenirNouvelId() -> int;

public:
    explicit SortieGrapheVisuelTexte(const std::string& cheminSortie);
    auto ajouterNoeud(const std::string& label) -> int;
    void ajouterArete(int parentId, int enfantId);
    void generer();
};

#endif /* ECD975BF_C219_4D88_AC4A_B12DE1902F2B */
