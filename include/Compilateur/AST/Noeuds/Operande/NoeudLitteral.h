#ifndef D2F8710F_A58B_4559_B0C0_4D8569EB08B9
#define D2F8710F_A58B_4559_B0C0_4D8569EB08B9
#pragma once

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/AmisVisiteurs.h"

class NoeudLitteral : public INoeud
{
    LISTE_DES_AMIS_VISITEURS
private:
    float _valeur;

public:
 
    explicit NoeudLitteral(float valeur);

    ~NoeudLitteral() = default;

    void accept(IVisiteur* visiteur) override;

    float getValeur() const { return _valeur; }
};


#endif /* D2F8710F_A58B_4559_B0C0_4D8569EB08B9 */
