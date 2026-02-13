#ifndef F158F3CA_48BA_406A_A611_87940EE9E015
#define F158F3CA_48BA_406A_A611_87940EE9E015
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include <memory>

class INoeud;
class VisiteurGeneralGenCode;
class GestionFonction;

class NoeudWhile : public IInstruction
{
    friend class VisiteurGeneralGenCode;
    friend class GestionFonction;
private: 
    std::shared_ptr<INoeud> noeudCondition;
    std::shared_ptr<INoeud> noeudBlocWhile;
    std::shared_ptr<INoeud> noeudBlocEndWhile;
    std::vector<std::shared_ptr<INoeud>> enfants;

public: 
    NoeudWhile();
    NoeudWhile(std::shared_ptr<INoeud>&& condition, std::shared_ptr<INoeud>&& blocWhile, std::shared_ptr<INoeud>&& blocEndWhile);
    ~NoeudWhile();
    
    void accept(IVisiteur* visiteur) override;
    void ajouterInstruction(std::shared_ptr<INoeud> enfant) override;
    [[nodiscard]] const std::vector<std::shared_ptr<INoeud>>& getEnfants() const { return enfants; }
};

#endif /* F158F3CA_48BA_406A_A611_87940EE9E015 */
