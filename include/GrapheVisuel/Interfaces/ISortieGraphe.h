#ifndef AA8FE33F_20FF_4F47_A33B_C39585B20FFE
#define AA8FE33F_20FF_4F47_A33B_C39585B20FFE

#include <string>
#include <vector>
class ISortieGraph
{
    private: 
        std::string _arbre; 

    public: 
        virtual void ajouterNoeudGraph(const std::string& parent, std::vector<std::string> enfants) = 0; 
};

#endif /* AA8FE33F_20FF_4F47_A33B_C39585B20FFE */
