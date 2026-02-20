#ifndef C6C14887_3564_44B6_93AF_297915A05DCF
#define C6C14887_3564_44B6_93AF_297915A05DCF

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <memory>

class IInstruction : public INoeud
{
public: 
    virtual ~IInstruction() = default;
    virtual void ajouterInstruction(INoeud* enfant) = 0; 
};

#endif /* C6C14887_3564_44B6_93AF_297915A05DCF */
