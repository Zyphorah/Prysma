#ifndef C6C14887_3564_44B6_93AF_297915A05DCF
#define C6C14887_3564_44B6_93AF_297915A05DCF

#include "Compiler/AST/Nodes/Interfaces/INode.h"

class IInstruction : public INode
{
public: 
    IInstruction() = default;
     ~IInstruction() override = default;
    IInstruction(const IInstruction&) = delete;
    auto operator=(const IInstruction&) -> IInstruction& = delete;
    IInstruction(IInstruction&&) = delete;
    auto operator=(IInstruction&&) -> IInstruction& = delete;
    virtual void addInstruction(INode* child) = 0; 
};

#endif /* C6C14887_3564_44B6_93AF_297915A05DCF */
