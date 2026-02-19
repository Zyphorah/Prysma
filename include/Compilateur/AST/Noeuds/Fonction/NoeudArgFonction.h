#ifndef B9F8948B_23A4_4812_A098_C71C4D92373B
#define B9F8948B_23A4_4812_A098_C71C4D92373B
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include <memory>
#include <string>
class NoeudArgFonction: public INoeud
{
private:
     std::shared_ptr<IType> _type;
     std::string _nom;

public: 
    NoeudArgFonction(std::shared_ptr<IType> type, std::string nom);
    ~NoeudArgFonction();

    void accept(IVisiteur* visiteur) override;
    const std::string& getNom() const { return _nom; }

    std::shared_ptr<IType> getType() const { return _type; }
};

#endif /* B9F8948B_23A4_4812_A098_C71C4D92373B */
