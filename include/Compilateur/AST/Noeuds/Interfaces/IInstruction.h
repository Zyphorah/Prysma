#ifndef C6C14887_3564_44B6_93AF_297915A05DCF
#define C6C14887_3564_44B6_93AF_297915A05DCF

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"

class IInstruction : public INoeud
{
public: 
    IInstruction() = default;
     ~IInstruction() override = default;
    IInstruction(const IInstruction&) = delete;
    auto operator=(const IInstruction&) -> IInstruction& = delete;
    IInstruction(IInstruction&&) = delete;
    auto operator=(IInstruction&&) -> IInstruction& = delete;
    virtual void ajouterInstruction(INoeud* enfant) = 0; 
};

#endif /* C6C14887_3564_44B6_93AF_297915A05DCF */
