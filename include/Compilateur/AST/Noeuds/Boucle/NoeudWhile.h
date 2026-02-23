#ifndef F158F3CA_48BA_406A_A611_87940EE9E015
#define F158F3CA_48BA_406A_A611_87940EE9E015

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"

class INoeud;
class VisiteurGeneralGenCode;
class GestionFonction;

class NoeudWhile : public NoeudInstruction
{
private: 
    INoeud* noeudCondition;
    INoeud* noeudBlocWhile;
    INoeud* noeudBlocFinWhile;

public: 
    NoeudWhile();
    NoeudWhile(INoeud* condition, INoeud* blocWhile, INoeud* blocEndWhile);
    ~NoeudWhile();
    
    void accept(IVisiteur* visiteur) override;

    INoeud* getNoeudCondition() const { return noeudCondition; }
    INoeud* getNoeudBlocWhile() const { return noeudBlocWhile; }
    INoeud* getNoeudBlocFinWhile() const { return noeudBlocFinWhile; }
};

#endif /* F158F3CA_48BA_406A_A611_87940EE9E015 */
