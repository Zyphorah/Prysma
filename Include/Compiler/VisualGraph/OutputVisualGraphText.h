#ifndef ECD975BF_C219_4D88_AC4A_B12DE1902F2B
#define ECD975BF_C219_4D88_AC4A_B12DE1902F2B

#include <string>
#include <sstream>

class OutputVisualGraphText
{
private:    
    std::string _cheminOutput;
    std::stringstream _contenuGraph;
    int _compteurId;
    auto obtenirNouvelId() -> int;

public:
    explicit OutputVisualGraphText(const std::string& cheminOutput);
    auto ajouterNode(const std::string& label) -> int;
    void ajouterArete(int parentId, int childId);
    void generatedr();
};

#endif /* ECD975BF_C219_4D88_AC4A_B12DE1902F2B */
